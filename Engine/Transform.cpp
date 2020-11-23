#include "Transform.h"
Transform::Transform() :position(float3::zero), rotation(float3::zero), scale(float3::one) {}
Transform::Transform(float3 pos, float3 rot, float3 scale) : position(pos), rotation(rot), scale(scale) {}
Transform::Transform(float3 pos, float3 rot) : position(pos), rotation(rot), scale(float3::one) {}
Transform::Transform(float3 pos) : position(pos), rotation(float3::zero), scale(float3::one) {}
Transform::~Transform() {}