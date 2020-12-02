#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

void GameObject::Update() {

}

Component* GameObject::CreateComponent(Component::ComponentType type) {

}

Component* GameObject::GetCoponentOfType(Component::ComponentType type) {
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it)->type == type) {
			return *it;
		}
	}
}
