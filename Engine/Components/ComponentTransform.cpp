#include "ComponentTransform.h"
#include "../GameObject.h"
#include "../ImGui/imgui.h"
#include "../Utilities/Leaks.h"
#include "../Application.h"
#include "../Modules/ModuleDebugDraw.h"


ComponentTransform::ComponentTransform(GameObject* anOwner, float3 pos, Quat rot, float3 scale) :Component(ComponentType::CTTransformation, anOwner), localPosition(pos), prevLocalPosition(pos), quatRotation(rot), prevQuatRotation(rot), localScale(scale), prevLocalScale(scale), prevRotDummy(float3::zero) {
	GenerateLocalMatrix();
	GenerateWorldMatrix();
}

ComponentTransform::~ComponentTransform() {

}

void ComponentTransform::Enable() {
	enabled = true;
}

void ComponentTransform::Update() {
	if (!enabled) return;

	if (
		localPosition.x != prevLocalPosition.x || localPosition.y != prevLocalPosition.y || localPosition.z != prevLocalPosition.z ||
		localScale.x != prevLocalScale.x || localScale.y != prevLocalScale.y || localScale.z != prevLocalScale.z ||
		quatRotation.x != prevQuatRotation.x || quatRotation.z != prevQuatRotation.z || quatRotation.y != prevQuatRotation.y || quatRotation.w != prevQuatRotation.w
		) {
		GenerateLocalMatrix();
		GenerateWorldMatrix();
		owner->OnTransformChanged(CalculateGlobalPosition(), CalculateGlobalRotation());
	}
}

void ComponentTransform::GenerateLocalMatrix() {
	localTransformationMatrix = float4x4::FromTRS(localPosition, quatRotation, localScale);
	prevLocalPosition = localPosition;
	prevLocalScale = localScale;
	prevQuatRotation = quatRotation;
}

void ComponentTransform::GenerateWorldMatrix() {
	transformationMatrix = localTransformationMatrix;
	if (owner != nullptr) {
		if (owner->parent != nullptr) {
			ComponentTransform* parentTransform = (ComponentTransform*)owner->parent->GetComponentOfType(ComponentType::CTTransformation);
			if (parentTransform != nullptr) {
				transformationMatrix = parentTransform->transformationMatrix * localTransformationMatrix;
			}
		}

		for (std::list<GameObject*>::iterator it = owner->children.begin(); it != owner->children.end(); ++it) {
			((ComponentTransform*)(*it)->GetComponentOfType(ComponentType::CTTransformation))->GenerateWorldMatrix();
		}
	}
}

void ComponentTransform::Disable() {
	enabled = false;
}

void ComponentTransform::DrawEditor() {
	if (ImGui::CollapsingHeader("Transform")) {

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Reset")) {
				Reset();
			}
			ImGui::EndPopup();
		}

		ImGui::DragFloat3("position", localPosition.ptr());
		//ImGui::DragFloat3("rotation", localPosition.ptr());

		float3 rotDummy = quatRotation.ToEulerXYZ();
		rotDummy = RadToDeg(rotDummy);

		ImGui::DragFloat3("Rotation", rotDummy.ptr());

		if (prevRotDummy.x != rotDummy.x || prevRotDummy.y != rotDummy.y || prevRotDummy.z != rotDummy.z) {

			//This triggers some weird interactions, may have to be modified 
			quatRotation = Quat::FromEulerXYZ(DegToRad(rotDummy.x), DegToRad(rotDummy.y), DegToRad(rotDummy.z));

			prevRotDummy = rotDummy;

		}

		ImGui::DragFloat3("scale", localScale.ptr());
	}
}

void ComponentTransform::DrawGizmos() {
	App->debugDraw->DrawAxisTriad(transformationMatrix);
}

void ComponentTransform::OnNewParent(GameObject* prevParent, GameObject* newParent) {
	//Get previous parent's transform
	ComponentTransform* prevParentGlobal = (ComponentTransform*)prevParent->GetComponentOfType(ComponentType::CTTransformation);
	if (prevParentGlobal != nullptr) {

		//Calculate new localPosition to keep the world position it had when attached to prevParent, but with the new parent
		float3 prevGlobalPos = prevParentGlobal->CalculateGlobalPosition() + localPosition;
		float3 globalPos = CalculateGlobalPosition() - localPosition;
		localPosition = prevGlobalPos - globalPos;

		//Same for scale
		float3 prevGlobalScale = prevParentGlobal->CalculateGlobalScale().Mul(localScale);
		float3 globalScale = CalculateGlobalScale().Div(localScale);
		localScale = prevGlobalScale.Div(globalScale);

		//Same for rotation
		Quat prevGlobalRot = prevParentGlobal->CalculateGlobalRotation() * quatRotation;
		Quat globalRot = CalculateGlobalRotation();

		//We inverse so that it is subtracted and not added (multiplication order may need to be switched)
		globalRot.Inverse();
		quatRotation = globalRot * prevGlobalRot * quatRotation;

	}
}


Quat ComponentTransform::CalculateGlobalRotation()const {
	if (owner->parent != nullptr) {
		Component* c = owner->parent->GetComponentOfType(ComponentType::CTTransformation);
		return (quatRotation * ((ComponentTransform*)c)->CalculateGlobalRotation());
	}
	return quatRotation;
}

float3 ComponentTransform::CalculateGlobalScale()const {

	if (owner->parent != nullptr) {
		Component* c = owner->parent->GetComponentOfType(ComponentType::CTTransformation);
		return localScale.Mul(((ComponentTransform*)c)->CalculateGlobalScale());
	}
	return localScale;
}

float3 ComponentTransform::CalculateGlobalPosition()const {

	if (owner->parent != nullptr) {
		Component* c = owner->parent->GetComponentOfType(ComponentType::CTTransformation);
		return ((ComponentTransform*)c)->CalculateGlobalPosition() + localPosition;
	}

	return localPosition;
}

float4x4 ComponentTransform::GetWorldMatrix()const {
	return transformationMatrix;
}

void ComponentTransform::Reset() {
	localPosition = float3::zero;
	quatRotation = Quat::identity;
	localScale = float3::one;
}