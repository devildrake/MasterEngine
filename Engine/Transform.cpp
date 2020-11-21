#include "Transform.h"
Transform::Transform() :position(float3::zero), rotation(float3::zero), scale(float3::one) {}
Transform::Transform(float3 pos, float3 rot, float3 scale) : position(pos), rotation(rot), scale(float3::one) {}
Transform::Transform(float3 pos, float3 rot) {}
Transform::Transform(float3 pos) {}
Transform::~Transform() {}