#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H
#include "Module.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>

class GameObject;
class Mesh;
class Skybox;

typedef unsigned int uint;

class ModuleScene :public Module {
private:
	GameObject* root;
	std::vector<GameObject*>gameObjects;
	Skybox* skybox;
public:
	ModuleScene();
	~ModuleScene();
	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);
	GameObject* GetRoot()const;
	UpdateStatus UpdateGameObject(GameObject* target);
	GameObject* LoadModel(std::string path);
	GameObject* ProcessNode(aiNode* node, const aiScene* scene, std::string path);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool DrawSkyBox();
	void DestroyGameObject(GameObject* go);
public:
	friend class PropertiesWindow;
};

#endif