#include "GameObject.h"

GameObject::GameObject() :parent(nullptr) {

}

GameObject::~GameObject() {

}

void GameObject::Update() {

}

Component* GameObject::CreateComponent(Component::ComponentType type) {
	return nullptr;
}

Component* GameObject::GetCoponentOfType(Component::ComponentType type) {
	/*if (components.size() == 0)return nullptr;
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it)->type == type) {
			return *it;
		}
	}*/
	return nullptr;
}
