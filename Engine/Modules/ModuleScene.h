#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H
#include "Module.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>

class GameObject;
class Mesh;

class ModuleScene :public Module {
private:
	GameObject* currentGameObject;
	GameObject* root;

	std::vector<GameObject*>gameObjects;

public:
	ModuleScene();
	~ModuleScene();
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	GameObject* CreateGameObject(const char* name);
	GameObject* GetRoot()const;
	update_status UpdateGameObject(GameObject* target);
	GameObject* LoadModel(std::string path);
	GameObject* ProcessNode(aiNode* node, const aiScene* scene, std::string path);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void UpdateGameObjectHierarchy();
	GameObject* GetGameObjectWithID(const int& id) const;

public:
	friend class PropertiesWindow;
};

#endif