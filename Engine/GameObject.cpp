#include "GameObject.h"
#include "Components/ComponentMeshRenderer.h"
#include "Components/ComponentTransform.h"
#include "Modules/ModuleScene.h"
#include "Utilities/Globals.h"

GameObject::GameObject() :parent(nullptr), name(""), id(-1), scene(nullptr) {

}

GameObject::GameObject(const char* name, GameObject* parentObject) : parent(parentObject), name(name), id(-1), scene(nullptr) {

}


GameObject::~GameObject() {

}

void GameObject::Update() {
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->Update();
	}
}

Component* GameObject::CreateComponent(Component::ComponentType type) {
	Component* ret = nullptr;
	switch (type) {
	case Component::CTTransformation:
		ret = new ComponentTransform(this);
		break;
	case Component::CTMeshRenderer:
		ret = new ComponentMeshRenderer(this);
		break;
	case Component::CTLight:
		break;
	default:
		break;
	}

	if (ret != nullptr) {
		components.push_back(ret);
	}

	return ret;
}

Component* GameObject::GetComponentOfType(Component::ComponentType type) {
	if (components.size() == 0)return nullptr;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it)->type == type) {
			return *it;
		}
	}
	return nullptr;
}

void GameObject::SetNewParent(GameObject* newParent) {

	if (!IsChild(newParent)) {
		GameObject* prevParent = parent;

		//parent->children.erase(this);
		parent->children.remove(this);
		parent = &(*newParent);

		parent->children.push_back(this);

		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
			(*it)->OnNewParent(prevParent, newParent);
		}

		scene->UpdateGameObjectHierarchy();
	}
}

void GameObject::UpdateID(int& id) {
	this->id = id;
	id++;
	for (std::list<GameObject*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->UpdateID(id);
	}

}

void GameObject::SetScene(ModuleScene* newScene) {
	scene = newScene;
}

int GameObject::GetID()const {
	return id;
}

bool GameObject::IsChild(GameObject* g)const {
	bool isChild = false;

	for (std::list<GameObject*>::const_iterator it = children.begin(); it != children.end() && !isChild; ++it) {
		if (*it == g) {
			isChild = true;
		}
		else {
			isChild = (*it)->IsChild(g);
		}
	}

	return isChild;
}
