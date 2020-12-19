#include "ComponentDirectionalLight.h"

ComponentDirectionalLight::ComponentDirectionalLight(GameObject* go, float3 direction) :ComponentLight(go, LightType::DIRECTIONAL), direction(direction) {

}

ComponentDirectionalLight::~ComponentDirectionalLight() {

}

void ComponentDirectionalLight::Enable() {

}

void ComponentDirectionalLight::Update() {

}

void ComponentDirectionalLight::Disable() {

}

void ComponentDirectionalLight::DrawEditor() {

}

void ComponentDirectionalLight::DrawGizmos() {

}

void ComponentDirectionalLight::OnNewParent(GameObject* prevParent, GameObject* newParent) {

}

void ComponentDirectionalLight::OnTransformModified() {

}