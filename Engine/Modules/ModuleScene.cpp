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
#include "../Components/ComponentCamera.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "ModuleEditor.h"
#include "../ImGuiWindows/PropertiesWindow.h"
#include "../Skybox.h"

ModuleScene::ModuleScene() : Module("Scene"), currentGameObject(nullptr), root(nullptr), skybox(nullptr) {
	//root = CreateGameObject("Root");
	//root->CreateComponent(Component::ComponentType::CTTransformation);
}
ModuleScene::~ModuleScene() {

}
bool ModuleScene::Init() {
	return true;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent) {
	GameObject* ret = new GameObject(name, this, parent);
	ret->SetScene(this);

	gameObjects.push_back(ret);
	return ret;
}

void ModuleScene::DestroyGameObject(GameObject* go) {
	std::vector<GameObject*>::iterator it = std::find(gameObjects.begin(), gameObjects.end(), go);
	if (it != gameObjects.end()) {
		gameObjects.erase(it);
	}

	App->editor->SetTargetObject(nullptr);
	//gameObjects.erase(gameObjects.begin() + go->GetSceneID());
	if (go->parent != nullptr) {
		go->RemoveFromParentsChildren();
	}
	RELEASE(go);
}

Mesh* ModuleScene::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	Mesh* newMesh = new Mesh(mesh);
	return newMesh;
}

bool ModuleScene::Start() {

	std::string faces[6] =
	{
		"Resources\\Skybox\\right.jpg",
			"Resources\\Skybox\\left.jpg",
			"Resources\\Skybox\\top.jpg",
			"Resources\\Skybox\\bottom.jpg",
			"Resources\\Skybox\\front.jpg",
			"Resources\\Skybox\\back.jpg"
	};

	skybox = new Skybox(faces, "Resources\\Shaders\\cubemap.vs", "Resources\\Shaders\\cubemap.fs");

	root = CreateGameObject("Root");

	ComponentTransform* firstTransform = (ComponentTransform*)root->CreateComponent(Component::ComponentType::CTTransformation);

	GameObject* firstChild = LoadModel("Resources\\Models\\BakerHouse.fbx");
	root->children.push_back(firstChild);
	firstChild->parent = root;

	currentGameObject = root;
	App->editorCamera->SetTargetGameObject(firstChild);

	GameObject* cameraObj = CreateGameObject("Camera", root);
	ComponentTransform* cameraTrans = (ComponentTransform*)cameraObj->CreateComponent(Component::ComponentType::CTTransformation);
	ComponentCamera* cameraComp = (ComponentCamera*)cameraObj->CreateComponent(Component::ComponentType::CTCamera);
	return true;
}

GameObject* ModuleScene::GetRoot()const {
	return root;
}

UpdateStatus ModuleScene::PreUpdate() {
	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::UpdateGameObject(GameObject* target) {

	target->Update();

	for (std::vector<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
		UpdateGameObject((*it));
	}

	return UPDATE_CONTINUE;

}

bool ModuleScene::DrawSkyBox() {
	if (skybox != nullptr) {
		skybox->Draw(App->editorCamera->GetFrustum()->ViewMatrix(), App->editorCamera->GetFrustum()->ProjectionMatrix());
		return true;
	}
	return false;
}


GameObject* ModuleScene::ProcessNode(aiNode* node, const aiScene* scene, std::string path) {


	GameObject* newObj = CreateGameObject(node->mName.C_Str());

	ComponentTransform* transform = (ComponentTransform*)newObj->CreateComponent(Component::ComponentType::CTTransformation);

	for (uint i = 0u; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* newMesh = ProcessMesh(mesh, scene);


		//Using the loaded transform from the model
		aiVector3D pos, scale;
		aiQuaternion rot;

		node->mTransformation.Decompose(scale, rot, pos);

		transform->localPosition = float3(pos.x, pos.y, pos.z);
		transform->quatRotation = Quat(rot.x, rot.y, rot.z, rot.w);
		transform->localScale = float3(scale.x, scale.y, scale.z);

		ComponentMeshRenderer* meshRenderer = (ComponentMeshRenderer*)newObj->CreateComponent(Component::ComponentType::CTMeshRenderer);
		meshRenderer->SetMesh(newMesh);
		Material* newMat = new Material(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], path);


		meshRenderer->SetMaterial(newMat);
	}

	//newObj->children.reserve(node->mNumChildren);

	// then do the same for each of its children
	for (uint i = 0u; i < node->mNumChildren; i++) {
		GameObject* newChild = ProcessNode(node->mChildren[i], scene, path);
		newObj->children.push_back(newChild);
		newChild->parent = newObj;
	}
	return newObj;
}

GameObject* ModuleScene::LoadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG("ERROR::ASSIMP:: %s", import.GetErrorString());
		return nullptr;
	}
	std::string modelDirectory = path.substr(0, path.find_last_of('/'));

	return (ProcessNode(scene->mRootNode, scene, path));
}

UpdateStatus ModuleScene::Update() {
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
						App->editorCamera->SetTargetGameObject(currentGameObject);
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
UpdateStatus ModuleScene::PostUpdate() {
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp() {
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
		RELEASE(*it);
	}
	RELEASE(skybox);
	return true;
}
