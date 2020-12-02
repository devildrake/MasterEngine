#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <string>
#include <vector>
#include "Components/Component.h"

class GameObject {
	std::string name;
	std::vector<Component*>components;

	GameObject* parent;
	std::vector<GameObject*>children;
public:
	GameObject();
	~GameObject();
	void Update();
	Component* CreateComponent(Component::ComponentType type);
	Component* GetCoponentOfType(Component::ComponentType type);
};

#endif