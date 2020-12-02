#include "Transform.h"
#include "../GameObject.h"

Transform::Transform() :localPosition(float3::zero), rotation(float3::zero), scale(float3::one) {

}


Transform::Transform(float3 pos, float3 rot, float3 scale) : localPosition(pos), rotation(rot), scale(scale) {

}


Transform::Transform(float3 pos, float3 rot) : localPosition(pos), rotation(rot), scale(float3::one) {

}


Transform::Transform(float3 pos) : localPosition(pos), rotation(float3::zero), scale(float3::one) {

}

Transform::~Transform() {

}

void Transform::Enable() {

}

void Transform::Update() {

}

void Transform::Disable() {

}

const float3 Transform::CalculatePosition()const {

	if (owner != nullptr) {
		Component* c = owner->GetCoponentOfType(ComponentType::Transformation);
		return ((Transform*)c)->CalculatePosition() + localPosition;
	}
	return localPosition;

	//return ((Transform*)(owner->GetCoponentOfType(ComponentType::Transformation)))->position;
}
