#include "GameObject.h"
#include "Components/ComponentMeshRenderer.h"
#include "Components/ComponentTransform.h"
#include "Components/ComponentCamera.h"
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
		break;
	case Component::CTCamera:
		ret = new ComponentCamera(this);
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
	if (newParent != nullptr) {
		if (!IsChild(newParent)) {
			GameObject* prevParent = parent;

			if (parent != nullptr) {
				parent->children.remove(this);
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

	for (std::list<GameObject*>::const_iterator it = children.begin(); it != children.end() && !isChild; ++it) {
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


void GameObject::OnTransformChanged(float3 newPos, Quat newRot) {
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->OnTransformModified(newPos, newRot);
	}
}