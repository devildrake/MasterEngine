#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <string>
#include <vector>
#include "../Components/Component.h"

class GameObject {
public:
	std::string name;
	std::vector<GameObject*>children;

private:
	GameObject* parent;
	std::vector<Component*>components;
	bool active;
	void _GetComponentsInChildrenOfType(Component::ComponentType type, std::vector<Component*>& retVec);

public:
	GameObject();
	GameObject(const char* name, GameObject* parentObject = nullptr);
	GameObject(const char* name, float3 pos, Quat rot, float3 scale, GameObject* parentObject = nullptr);

	~GameObject();
	void Update();
	Component* CreateComponent(Component::ComponentType type, int additionalParam = 0);
	Component* GetComponentOfType(Component::ComponentType type);
	Component* GetComponentInChildrenOfType(Component::ComponentType type);
	std::vector<Component*> GetComponentsInChildrenOfType(Component::ComponentType type);

	//This method is used to remove myself from my parent's children list when I'm destroyed
	void GameObject::RemoveFromParentsChildren();


	void GameObject::SetNewParent(GameObject* newParent);
	bool IsChild(GameObject* g)const;
	bool IsActive()const;
	void SetActive(bool a);
	void DrawGizmos()const;
	void OnTransformChanged();
	GameObject* GetParent();
public:
	friend class PropertiesWindow;

};

#endif