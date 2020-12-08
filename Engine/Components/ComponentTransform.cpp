#include "ComponentTransform.h"
#include "../GameObject.h"
#include "../ImGui/imgui.h"


ComponentTransform::ComponentTransform(GameObject* anOwner, float3 pos, Quat rot, float3 scale) :Component(ComponentType::CTTransformation, anOwner), localPosition(pos), prevLocalPosition(pos), quatRotation(rot), prevQuatRotation(rot), localScale(scale), prevLocalScale(scale) {
	transformationMatrix = float4x4::Translate(CalculateGlobalPosition()) * float4x4::FromQuat(quatRotation) * float4x4::Scale(localScale) * float4x4::identity;
}

ComponentTransform::~ComponentTransform() {

}

void ComponentTransform::Enable() {
	enabled = true;
}

void ComponentTransform::Update() {
	if (!enabled) return;

	//TO DO OPTIMIZE CALLS SO IT ONLY UPDATES WHENEVER TRANSFORM WAS CHANGED
	//if (
		/*localPosition.x != prevLocalPosition.x || localPosition.y != prevLocalPosition.y || localPosition.z != prevLocalPosition.z ||
		localScale.x != prevLocalScale.x || localScale.y != prevLocalScale.y || localScale.z != prevLocalScale.z ||
		quatRotation.x != quatRotation.x || quatRotation.z != quatRotation.z || quatRotation.y != quatRotation.y || quatRotation.w != quatRotation.w
		) {*/
	transformationMatrix = float4x4::FromTRS(CalculateGlobalPosition(), CalculateGlobalRotation(), CalculateGlobalScale());
	//}

}

void ComponentTransform::Disable() {
	enabled = false;
}

void ComponentTransform::DrawEditor() {
	ImGui::DragFloat3("position", localPosition.ptr());
	//ImGui::DragFloat3("rotation", localPosition.ptr());

	float3 rotDummy = quatRotation.ToEulerXYZ();
	rotDummy = RadToDeg(rotDummy);
	ImGui::DragFloat3("Rotation", rotDummy.ptr());

	ImGui::DragFloat3("scale", localScale.ptr());

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
		//globalRot.Inverse();
		//quatRotation = prevGlobalRot * globalRot;

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

float4x4 ComponentTransform::GetTransformMatrix()const {
	return transformationMatrix;
}

