#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "../Components/ComponentMeshRenderer.h"
#include "../Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleTextures.h"
#include "../GameObject.h"
#include "assimp/postprocess.h"
#include "../Components/ComponentTransform.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"

ModuleScene::ModuleScene() :currentGameObject(nullptr) {
	//root = CreateGameObject("Root");
	//root->CreateComponent(Component::ComponentType::CTTransformation);
}
ModuleScene::~ModuleScene() {
	
}
bool ModuleScene::Init() {
	return true;
}

GameObject* ModuleScene::CreateGameObject(const char* name) {
	GameObject* ret = new GameObject(name);
	ret->SetScene(this);

	gameObjects.push_back(ret);
	return ret;
}

Mesh* ModuleScene::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	Mesh* newMesh = new Mesh(mesh);
	return newMesh;
}

bool ModuleScene::Start() {

	root = CreateGameObject("Root");

	ComponentTransform* firstTransform = (ComponentTransform*)root->CreateComponent(Component::ComponentType::CTTransformation);

	GameObject* firstChild = LoadModel("Resources\\Models\\BakerHouse.fbx");
	root->children.push_back(firstChild);
	firstChild->parent = root;

	currentGameObject = root;
	App->editorCamera->SetTargetModel(firstTransform);
	UpdateGameObjectHierarchy();
	return true;
}

GameObject* ModuleScene::GetRoot()const {
	return root;
}

update_status ModuleScene::PreUpdate() {
	return UPDATE_CONTINUE;
}

update_status ModuleScene::UpdateGameObject(GameObject* target) {

	target->Update();

	for (std::list<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
		UpdateGameObject((*it));
	}

	return UPDATE_CONTINUE;

}

GameObject* ModuleScene::ProcessNode(aiNode* node, const aiScene* scene, std::string path) {


	GameObject* newObj = CreateGameObject(node->mName.C_Str());

	ComponentTransform* transform = (ComponentTransform*)newObj->CreateComponent(Component::ComponentType::CTTransformation);

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* newMesh = ProcessMesh(mesh, scene);


		//Using the loaded transform from the model
		aiVector3D pos, scale;
		aiQuaternion rot;

		node->mTransformation.Decompose(scale, rot, pos);

		transform->localPosition = float3(pos.x, pos.y, pos.z);
		transform->quatRotation = Quat(rot.x, rot.y, rot.z, rot.w);
		transform->localScale = float3(scale.x, scale.y, scale.z);


		//Setting name of the gameObject
		//newObj->name = mesh->mName.C_Str();
		ComponentMeshRenderer* meshRenderer = (ComponentMeshRenderer*)newObj->CreateComponent(Component::ComponentType::CTMeshRenderer);
		meshRenderer->SetMesh(newMesh);
		//aiString file, std::string& materialPath, std::string modelPath);
		Material* newMat = new Material(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], path);


		meshRenderer->SetMaterial(newMat);
	}

	//newObj->children.reserve(node->mNumChildren);

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		GameObject* newChild = ProcessNode(node->mChildren[i], scene, path);
		newObj->children.push_back(newChild);
		newChild->parent = newObj;
	}
	return newObj;
}

GameObject* ModuleScene::LoadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG("ERROR::ASSIMP:: %s", import.GetErrorString());
		return nullptr;
	}
	std::string modelDirectory = path.substr(0, path.find_last_of('/'));

	return (ProcessNode(scene->mRootNode, scene, path));
}

update_status ModuleScene::Update() {
	//std::string lastFile = App->input->GetLastFileDroppedOnWindow();
	/*if (lastFile.size() > 0) {
		if (currentGameObject != nullptr) {
			//If a new file has been dropped and it doesn't share the name with the current model, try to load it as model or texture
			if (currentGameObject->GetFileName() != lastFile) {
				Model* newModel = new Model();
				const aiScene* tempScene = nullptr;
				//Scene Found will tell wether or not a model could be loaded
				if (Model::SceneFound(lastFile.c_str(), tempScene)) {

					App->renderer->RemoveModel(currentGameObject);
					delete currentGameObject;
					if (newModel->Load(lastFile.c_str(), tempScene)) {
						currentGameObject = newModel;
						App->renderer->AddModel(currentGameObject);
						App->editorCamera->SetTargetModel(currentGameObject);
					}
					else {
						currentGameObject = nullptr;
						delete newModel;
					}
				}
				else {
					LOG("%s couldn't be loaded as a model", lastFile.c_str());

					delete newModel;
					GLuint possibleTexture;
					std::pair<int, int>possibleSize;
					if (lastFile != currentGameObject->GetMaterials()[0].GetTexturePath()) {
						if (App->textures->LoadTexture(lastFile, possibleTexture, possibleSize)) {
							LOG("%s was found to be a texture-compatible file, loading as diffuse for currentGameObject", lastFile.c_str());
							currentGameObject->ReleaseTextures();

							int lastSlash = 0;

							for (int i = lastFile.size(); i > 0 && lastSlash == 0; i--) {
								if (lastFile[i] == '\\') {
									lastSlash = i;
								}
							}

							currentGameObject->LoadMaterial(lastFile.substr(lastSlash, lastFile.size() - 1).c_str(), lastFile, possibleTexture, possibleSize);
						}
					}
					else {
						LOG("INFO --> The same file won't be loaded twice, Application will do nothing.");
					}
				}
			}
			else {
				LOG("INFO --> The same file won't be loaded twice, Application will do nothing.");
			}
		}
	}
	*/

	UpdateGameObject(root);

	return UPDATE_CONTINUE;
}
update_status ModuleScene::PostUpdate() {
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp() {

	if (currentGameObject != nullptr) {
		//App->renderer->RemoveModel(currentGameObject);
		delete currentGameObject;
		currentGameObject = nullptr;
	}

	return true;
}

void ModuleScene::UpdateGameObjectHierarchy() {
	int id = 0;
	root->UpdateID(id);
}

GameObject* ModuleScene::GetGameObjectWithID(const int& id) const {
	GameObject* ret = nullptr;
	for (std::vector<GameObject*>::const_iterator it = gameObjects.begin(); it != gameObjects.end() && ret == nullptr; ++it) {
		if ((*it)->GetID() == id) {
			ret = *it;
		}
	}
	return ret;
}
