#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "MathGeoLib/Math/float3.h"
class Transform {
public:
	float3 position;
	float3 rotation;
	float3 scale;
public:
	Transform();
	Transform(float3, float3, float3);
	Transform(float3, float3);
	Transform(float3);
	~Transform();
};

#endif