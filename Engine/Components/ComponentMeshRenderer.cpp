#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "../GameObject.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"

ComponentMeshRenderer::ComponentMeshRenderer(GameObject* anOwner) :Component(ComponentType::CTMeshRenderer, anOwner), mesh(nullptr), material(nullptr) {

}

ComponentMeshRenderer::~ComponentMeshRenderer() {

}

void ComponentMeshRenderer::Enable() {

}

void ComponentMeshRenderer::Update() {
	mesh->Draw(*material, ((ComponentTransform*)(owner->GetComponentOfType(ComponentType::CTTransformation)))->GetTransformMatrix());
}

void ComponentMeshRenderer::Disable() {

}

void ComponentMeshRenderer::SetMesh(Mesh* aMesh) {
	mesh = aMesh;
}

void ComponentMeshRenderer::SetMaterial(Material* aMat) {
	material = aMat;
}