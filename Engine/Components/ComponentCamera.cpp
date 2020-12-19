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


ComponentCamera::ComponentCamera(GameObject* anOwner, float aNearPDistance, float aFarPDistance) : Component(ComponentType::CTCamera, anOwner), nearPlaneDistance(aNearPDistance), farPlaneDistance(aFarPDistance), aspectRatio(1.77f) {
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	frustum.SetPos(((ComponentTransform*)(owner->GetComponentOfType(ComponentType::CTTransformation)))->CalculateGlobalPosition());
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * 90), (float)App->window->GetWidth() / (float)App->window->GetHeight());
	//frustum.SetWorldMatrix(((ComponentTransform*)(owner->GetComponentOfType(ComponentType::CTTransformation)))->GetWorldMatrix());
}

void ComponentCamera::DrawGizmos() {
	if (App->debugDraw != nullptr) {
		App->debugDraw->DrawFrustum(frustum);
	}
}

void ComponentCamera::OnTransformModified(float3 newPos, Quat newRot) {
	frustum.SetPos(newPos);
	frustum.SetFront(newRot * float3::unitZ);
	frustum.SetUp(newRot * float3::unitY);
}
