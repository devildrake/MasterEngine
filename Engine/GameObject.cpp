#include "GameObject.h"
#include "Components/ComponentMeshRenderer.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
#include "Components/ComponentLight.h"
#include "Modules/ModuleScene.h"
#include "Utilities/Globals.h"

GameObject::GameObject() :parent(nullptr), name(""), scene(nullptr), active(true) {

}

GameObject::GameObject(const char* name, ModuleScene* scene, GameObject* parentObject) : parent(nullptr), name(name), scene(scene), active(true) {
	SetNewParent(parentObject);
}

GameObject::~GameObject() {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		RELEASE(*it);
	}
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
		ret = new ComponentLight(this);
		break;
	case Component::CTCamera:
		ret = new ComponentCamera(this, 0.1f, 200);
		break;
	default:
		break;
	}

	if (ret != nullptr) {
		components.push_back(ret);
	}

	return ret;
}

std::vector<Component*>GameObject::GetComponentsInChildrenOfType(Component::ComponentType type) {
	std::vector<Component*> retVec;

	Component* mySelf = GetComponentOfType(type);
	if (mySelf != nullptr) {
		retVec.push_back(mySelf);
	}

	if (children.size() > 0) {
		std::vector<Component*> currChildRet;
		for (std::vector<GameObject*>::const_iterator it = children.begin(); it != children.end(); ++it) {
			currChildRet = (*it)->GetComponentsInChildrenOfType(type);

			if (currChildRet.size() > 0) {
				retVec.insert(retVec.end(), currChildRet.begin(), currChildRet.end());
			}
		}

	}

	return retVec;
}


Component* GameObject::GetComponentInChildrenOfType(Component::ComponentType type) {
	Component* retComp = nullptr;

	if (components.size() > 0)
		retComp = GetComponentOfType(type);

	if (retComp == nullptr && children.size() > 0) {
		for (std::vector<GameObject*>::const_iterator it = children.begin(); it != children.end() && retComp == nullptr; ++it) {
			retComp = GetComponentInChildrenOfType(type);
		}
	}
	return retComp;
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

void GameObject::RemoveFromParentsChildren() {
	std::vector<GameObject*>::iterator myItAtParent;
	bool found = false;
	for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end() && !found; ++it) {
		if (*it == this) {
			myItAtParent = it;
			found = true;
		}
	}

	if (parent != nullptr) {
		parent->children.erase(myItAtParent);
	}
}

void GameObject::SetNewParent(GameObject* newParent) {
	if (newParent != nullptr) {
		if (!IsChild(newParent)) {
			GameObject* prevParent = parent;
			if (prevParent != nullptr) {
				RemoveFromParentsChildren();
			}
			parent = &(*newParent);

			parent->children.push_back(this);

			for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
				(*it)->OnNewParent(prevParent, newParent);
			}

			//scene->UpdateGameObjectHierarchy();
		}
	} else {
		parent = nullptr;
	}
}

void GameObject::SetScene(ModuleScene* newScene) {
	scene = newScene;
}

bool GameObject::IsChild(GameObject* g)const {
	bool isChild = false;

	for (std::vector<GameObject*>::const_iterator it = children.begin(); it != children.end() && !isChild; ++it) {
		if (*it == g) {
			isChild = true;
		} else {
			isChild = (*it)->IsChild(g);
		}
	}

	return isChild;
}

void GameObject::DrawGizmos()const {
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->DrawGizmos();
	}
}


void GameObject::OnTransformChanged() {

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->OnTransformChanged();
	}

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->OnTransformModified();
	}

}