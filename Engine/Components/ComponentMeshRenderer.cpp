#include "ComponentMeshRenderer.h"
#include "../ImGui/imgui.h"


#include "ComponentTransform.h"
#include "../GameObject.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "../Utilities/Leaks.h"
#include "../Utilities/GLOBALS.h"
#include "../Application.h"
#include "../Modules/ModuleDebugDraw.h"
#include "../Components/ComponentTransform.h"
//#include "../ImGui/imgui.h"
ComponentMeshRenderer::ComponentMeshRenderer(GameObject* anOwner) :Component(ComponentType::CTMeshRenderer, anOwner), mesh(nullptr), material(nullptr) {

}

ComponentMeshRenderer::~ComponentMeshRenderer() {
	RELEASE(mesh);
	RELEASE(material);
}

void ComponentMeshRenderer::Enable() {

}

void ComponentMeshRenderer::Update() {
	mesh->Draw(*material, ((ComponentTransform*)(owner->GetComponentOfType(ComponentType::CTTransformation)))->GetWorldMatrix());
}

void ComponentMeshRenderer::Disable() {

}

void ComponentMeshRenderer::GenerateAABB() {
	localOrientedBoundingBox = mesh->GetAABB().ToOBB();
	ComponentTransform* transform = (ComponentTransform*)owner->GetComponentOfType(CTTransformation);
	localOrientedBoundingBox.Transform(transform->GetWorldMatrix());
	localAxisAlignedBoundingBox = localOrientedBoundingBox.MinimalEnclosingAABB();
}

const AABB& ComponentMeshRenderer::GetAABB()const {
	return localAxisAlignedBoundingBox;
}

void ComponentMeshRenderer::SetMesh(Mesh* aMesh) {
	mesh = aMesh;
	GenerateAABB();
}

void ComponentMeshRenderer::SetMaterial(Material* aMat) {
	material = aMat;
}

void ComponentMeshRenderer::DrawEditor() {
	if (ImGui::CollapsingHeader("Mesh Renderer")) {

		ImGui::Text("Vertices");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->GetVertices());

		ImGui::Text("Triangles");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->GetTris());
		ImGui::Spacing();
		ImGui::Text("Texture Info");

		ImGui::Text("Texture Name: %s", material->GetTextureName().c_str());
		ImGui::Text("Texture Path: %s", material->GetTexturePath().c_str());
		ImTextureID id = ImTextureID(material->GetTextureID());

		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

		ImGui::Image(ImTextureID(material->GetTextureID()), ImVec2(material->GetTextureSize().first / 16, material->GetTextureSize().second / 16), uv_min, uv_max, tint_col, border_col);
		ImGui::Text("%d x %d", material->GetTextureSize().first, material->GetTextureSize().second);

	}
}

void ComponentMeshRenderer::DrawGizmos() {
	if (App->debugDraw != nullptr) {
		App->debugDraw->DrawAABB(localAxisAlignedBoundingBox);
	}
}

void ComponentMeshRenderer::OnTransformModified(float3 newPos, Quat newRot) {
	GenerateAABB();
}
