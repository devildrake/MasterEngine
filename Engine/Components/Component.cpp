#include "Component.h"
#include "../GameObject.h"
#include "../ImGui/imgui.h"
#include "../Utilities/Leaks.h"


Component::Component(ComponentType aType, GameObject* anOwner) : enabled(true), owner(anOwner), type(aType) {
	name = ComponentTypeToString(type);
}

Component::~Component() {

}

void Component::Enable() {

}

void Component::Update() {

}

void Component::Disable() {

}

std::string Component::ComponentTypeToString(ComponentType type) const {
	switch (type) {
	case Component::CTTransformation:
		return "Transform";
		break;
	case Component::CTMeshRenderer:
		return "Mesh Renderer";
		break;
	case Component::CTLight:
		return "Light";
		break;
	default:
		return "";
		break;
	}
}

void Component::DrawEditor() {
	ImGui::Text(name.c_str());
}

void Component::OnNewParent(GameObject* prevParent, GameObject* newParent) {

}

void Component::DrawGizmos() {
}


void Component::OnTransformModified(float3 newPos, Quat newRot) {

}

void Component::OnSave() {

}
