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
#include "../Components/ComponentLight.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "ModuleEditor.h"
#include "../ImGuiWindows/PropertiesWindow.h"
#include "../Skybox.h"

ModuleScene::ModuleScene() : Module("Scene"), root(nullptr), skybox(nullptr) {
	//root = CreateGameObject("Root");
	//root->CreateComponent(Component::ComponentType::CTTransformation);
}
ModuleScene::~ModuleScene() {

}
bool ModuleScene::Init() {
	return true;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent) {
	GameObject* ret = new GameObject(name, parent);
	//ret->SetScene(this);

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
	if (go->GetParent() != nullptr) {
		go->RemoveFromParentsChildren();
	}
	RELEASE(go);
}

Mesh* ModuleScene::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	Mesh* newMesh = new Mesh(mesh);
	return newMesh;
}

bool ModuleScene::Start() {
	//LOAD SCENE
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
	firstChild->SetNewParent(root);
	//root->children.push_back(firstChild);
	//firstChild->parent = root;

	App->editorCamera->SetTargetGameObject(firstChild);

	GameObject* cameraObj = CreateGameObject("Camera", root);
	ComponentTransform* cameraTrans = (ComponentTransform*)cameraObj->CreateComponent(Component::ComponentType::CTTransformation);
	ComponentCamera* cameraComp = (ComponentCamera*)cameraObj->CreateComponent(Component::ComponentType::CTCamera);


	GameObject* newObjParent = CreateGameObject("parent", root);

	newObjParent->CreateComponent(Component::ComponentType::CTLight);

	std::string newName = "child" + std::to_string(0);
	GameObject* firstChildA = CreateGameObject(newName.c_str(), newObjParent);
	firstChildA->CreateComponent(Component::ComponentType::CTLight);

	//newName = "grandChild" + std::to_string(0);
	//GameObject* grandfirstChildA = CreateGameObject(newName.c_str(), firstChildA);
	//grandfirstChildA->CreateComponent(Component::ComponentType::CTLight);


	for (uint i = 0u; i < 4u; ++i) {
		newName = "child" + std::to_string(i + 1);
		GameObject* newObj = CreateGameObject(newName.c_str(), newObjParent);
		newObj->CreateComponent(Component::ComponentType::CTLight, ComponentLight::LightType::POINT);
	}

	for (uint i = 0u; i < 4u; ++i) {
		newName = "child" + std::to_string(i + 1);
		GameObject* newObj = CreateGameObject(newName.c_str(), firstChildA);
		newObj->CreateComponent(Component::ComponentType::CTLight);
	}

	std::vector<Component*>lights = newObjParent->GetComponentsInChildrenOfType(Component::ComponentType::CTLight);

	LOG("Found %d lightComponents", lights.size());

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
		skybox->Draw(App->editorCamera->GetFrustum().ViewMatrix(), App->editorCamera->GetFrustum().ProjectionMatrix());
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
		//newObj->children.push_back(newChild);
		newChild->SetNewParent(newObj);
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

	UpdateGameObject(root);

	return UPDATE_CONTINUE;
}
UpdateStatus ModuleScene::PostUpdate() {
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp() {
	RELEASE(root);
	//for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
	//	RELEASE(*it);
	//}
	RELEASE(skybox);
	return true;
}
