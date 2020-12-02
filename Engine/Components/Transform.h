#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "../MathGeoLib/Math/float3.h"
#include "../MathGeoLib/Math/Quat.h"
#include "Component.h"

class Transform :public Component {

public:
	Transform();
	Transform(float3, float3, float3);
	Transform(float3, float3);
	Transform(float3);
	~Transform();

	void Enable() override;
	void Update()override;
	void Disable()override;
	const float3 CalculatePosition()const;
public:
	float3 localPosition;
	//float3 position() const {
	//	return CalculatePosition();
	//}

	float3 rotation;
	float3 scale;
	Quat quatRotation;
};

#endif