#include "ComponentCamera.h"
#include "../ImGui/imgui.h"
#include "../Utilities/Leaks.h"
#include "../Application.h"
#include "../Modules/ModuleDebugDraw.h"
#include "../Modules/ModuleWindow.h"
#include "../GameObject.h"
#include "ComponentTransform.h"

void ComponentCamera::Enable() {

}

void ComponentCamera::Update() {

}

void ComponentCamera::Disable() {

}

void ComponentCamera::DrawEditor() {

}

void ComponentCamera::OnNewParent(GameObject* prevParent, GameObject* newParent) {

}

Frustum& ComponentCamera::GetFrustum() {
	return frustum;
}



ComponentCamera::~ComponentCamera() {
}
//
void ComponentCamera::SetUpFrustum(float nearDistance, float farDistance) {
	ComponentTransform* transform = (ComponentTransform*)owner->GetComponentOfType(Component::ComponentType::CTTransformation);

	//aspectRatio = (float)App->window->GetWidth() / (float)App->window->GetHeight();

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearDistance, farDistance);
	//SetAspectRatio(aspectRatio);
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(90), 1.77f);
	frustum.SetPos(transform->localPosition);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	//camera->SetFrustum(frustum);
	//frustum = nullptr;
}


//void ComponentCamera::SetUpFrustum() {
//	//aspectRatio = (float)App->window->GetWidth() / (float)App->window->GetHeight();
//	frustum = new Frustum();
//	frustum->SetKind(FrustumSpaceGL, FrustumRightHanded);
//	frustum->SetViewPlaneDistances(0, 200);
//	frustum->SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.77f);
//	//SetAspectRatio(aspectRatio);
//	ComponentTransform* transform = (ComponentTransform*)owner->GetComponentOfType(Component::ComponentType::CTTransformation);
//
//	frustum->SetPos(transform->localPosition);
//	frustum->SetFront(float3::unitZ);
//	frustum->SetUp(float3::unitY);
//	//camera->GetFrustum() = &frustum;
//}



ComponentCamera::ComponentCamera(GameObject* anOwner, float aNearPDistance, float aFarPDistance) : Component(ComponentType::CTCamera, anOwner) {

	SetUpFrustum(aNearPDistance, aFarPDistance);


	//LOG("Created Camera with planes %f %f, position %f, front %f, up %f, right %f, fov %f and ar %f", aNearPDistance, aFarPDistance, frustum.Pos(), frustum.Front().ptr(), frustum.Up().ptr(), frustum.WorldRight().ptr(), DEGTORAD * 90, 1.77f);
}
//frustum.SetPos(((ComponentTransform*)(owner->GetComponentOfType(ComponentType::CTTransformation)))->CalculateGlobalPosition());

void ComponentCamera::DrawGizmos() {
	if (App->debugDraw != nullptr) {
		App->debugDraw->DrawFrustum(frustum);
	}
}

void ComponentCamera::OnTransformModified() {

	ComponentTransform* transform = (ComponentTransform*)owner->GetComponentOfType(Component::ComponentType::CTCamera);

	frustum.SetPos(transform->CalculateGlobalPosition());
	frustum.SetFront(transform->CalculateGlobalRotation() * float3::unitZ);
	frustum.SetUp(transform->CalculateGlobalRotation() * float3::unitY);
}
