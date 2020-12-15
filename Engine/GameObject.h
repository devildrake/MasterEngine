#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <string>
#include <list>
#include "Components/Component.h"
#include <vector>

class ModuleScene;
class GameObject {
public:
	std::string name;
	GameObject* parent;
	std::list<GameObject*>children;
	bool active;
private:
	std::vector<Component*>components;
	ModuleScene* scene;
public:
	GameObject();
	GameObject(const char* name, ModuleScene* scene = nullptr, GameObject* parentObject = nullptr);
	~GameObject();
	void Update();
	Component* CreateComponent(Component::ComponentType type);
	Component* GetComponentOfType(Component::ComponentType type);
	void GameObject::SetNewParent(GameObject* newParent);
	void SetScene(ModuleScene* newScene);
	int GetID()const;
	bool IsChild(GameObject* g)const;
	void DrawGizmos()const;
	void OnTransformChanged(float3 newPos, Quat newRot);

public:
	friend class PropertiesWindow;

};

#endif