#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "../MathGeoLib/MathGeoLib.h"
#include "Component.h"

class ComponentTransform :public Component {
public:
	float3 localPosition;
	float3 localScale;
	Quat quatRotation;
private:
	float4x4 transformationMatrix;
	float3 prevLocalPosition;
	float3 prevLocalScale;
	Quat   prevQuatRotation;
public:
	ComponentTransform(GameObject* anOwner, float3 pos = float3::zero, Quat rotation = Quat::identity, float3 scale = float3::one);
	~ComponentTransform();

	void Enable() override;
	void Update()override;
	void Disable()override;
	void DrawEditor() override;
	void OnNewParent(GameObject* prevParent, GameObject* newParent)override;

	float3 CalculateGlobalPosition()const;
	Quat CalculateGlobalRotation()const;
	float3 CalculateGlobalScale()const;
	float4x4 GetTransformMatrix()const;
};

#endif