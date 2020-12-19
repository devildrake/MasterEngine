#ifndef COMPONENT_CAMERA_H
#define COMPONENT_CAMERA_H
#include "Component.h"
#include "../MathGeoLib/Geometry/Frustum.h"
class ComponentCamera :public Component {
private:
	Frustum frustum;
	float aspectRatio;
	float nearPlaneDistance, farPlaneDistance;
public:
	ComponentCamera(GameObject* anOwner, float aNearPDistance, float aFarPDistance);
	void Enable() override;
	void Update()override;
	void Disable()override;
	void DrawEditor() override;
	void DrawGizmos() override;
	void OnNewParent(GameObject* prevParent, GameObject* newParent)override;
	void OnTransformModified(float3 newPos, Quat newRot)override;
	Frustum& GetFrustum();


};

#endif