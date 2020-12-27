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
		owner->OnTransformChanged();



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
		if (owner->GetParent() != nullptr) {
			ComponentTransform* parentTransform = (ComponentTransform*)owner->GetParent()->GetComponentOfType(ComponentType::CTTransformation);
			if (parentTransform != nullptr) {
				transformationMatrix = parentTransform->transformationMatrix * localTransformationMatrix;
			}
		}

		for (std::vector<GameObject*>::iterator it = owner->children.begin(); it != owner->children.end(); ++it) {
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
	//Get previous and new parents' transform
	if (prevParent == nullptr)return;
	ComponentTransform* prevParentTransform = (ComponentTransform*)prevParent->GetComponentOfType(ComponentType::CTTransformation);
	ComponentTransform* newParentTransform = (ComponentTransform*)newParent->GetComponentOfType(ComponentType::CTTransformation);
	if (prevParentTransform != nullptr) {

		//To keep the local values untouched, we add the previous parent's global position to the localPosition and remove the current's global position from the localPosition
		float3 prevGlobalPos = prevParentTransform->CalculateGlobalPosition() + localPosition;
		float3 globalPos = CalculateGlobalPosition() - localPosition;
		localPosition = prevGlobalPos - globalPos;

		//Same for scale
		float3 prevGlobalScale = prevParentTransform->CalculateGlobalScale().Mul(localScale);
		float3 globalScale = CalculateGlobalScale().Div(localScale);
		localScale = prevGlobalScale.Div(globalScale);

		//Same for rotation
		Quat prevGlobalRot = prevParentTransform->CalculateGlobalRotation();
		Quat newGlobalRot = newParentTransform->CalculateGlobalRotation();

		//We inverse so that it is subtracted and not added (multiplication order may need to be switched)
		newGlobalRot.Inverse();
		quatRotation = newGlobalRot * prevGlobalRot * quatRotation;

	}
}


Quat ComponentTransform::CalculateGlobalRotation()const {
	if (owner->GetParent() != nullptr) {
		Component* c = owner->GetParent()->GetComponentOfType(ComponentType::CTTransformation);
		if (c != nullptr) {
			return (quatRotation * ((ComponentTransform*)c)->CalculateGlobalRotation());
		}
	}
	return quatRotation;
}

float3 ComponentTransform::CalculateGlobalScale()const {

	if (owner->GetParent() != nullptr) {
		Component* c = owner->GetParent()->GetComponentOfType(ComponentType::CTTransformation);
		if (c != nullptr) {
			return localScale.Mul(((ComponentTransform*)c)->CalculateGlobalScale());
		}
	}
	return localScale;
}

float3 ComponentTransform::CalculateGlobalPosition()const {

	if (owner->GetParent() != nullptr) {
		Component* c = owner->GetParent()->GetComponentOfType(ComponentType::CTTransformation);
		if (c != nullptr) {
			return ((ComponentTransform*)c)->CalculateGlobalPosition() + localPosition;
		}
	}

	return localPosition;
}

const float4x4& ComponentTransform::GetWorldMatrix()const {
	return transformationMatrix;
}

void ComponentTransform::Reset() {
	localPosition = float3::zero;
	quatRotation = Quat::identity;
	localScale = float3::one;
}

void ComponentTransform::SetPosition(float3 newGlobalPos) {
	float3 globalPos = CalculateGlobalPosition();
	float3 globalMovement = newGlobalPos - globalPos;

	localPosition += globalMovement;

}