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
private:
	int id;
	std::vector<Component*>components;
	ModuleScene* scene;
public:
	GameObject();
	GameObject(const char* name, GameObject* parentObject = nullptr);
	~GameObject();
	void Update();
	Component* CreateComponent(Component::ComponentType type);
	Component* GetComponentOfType(Component::ComponentType type);
	void GameObject::SetNewParent(GameObject* newParent);
	void UpdateID(int& id);
	void SetScene(ModuleScene* newScene);
	int GetID()const;
	bool IsChild(GameObject* g)const;

public:
	friend class PropertiesWindow;

};

#endif