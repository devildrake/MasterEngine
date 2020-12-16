#ifndef COMPONENT_DIRECTIONAL_LIGHT_H
#define COMPONENT_DIRECTIONAL_LIGHT_H
#include "ComponentLight.h"
class ComponentDirectionalLight :public ComponentLight {
private:
	float3 direction;
public:
	ComponentDirectionalLight(GameObject* go, float3 direction);
	~ComponentDirectionalLight();
	void Enable() override;
	void Update()override;
	void Disable()override;
	void DrawEditor() override;
	void DrawGizmos() override;
	void OnNewParent(GameObject* prevParent, GameObject* newParent)override;
	void OnTransformModified(float3 newPos, Quat newRot)override;

};

#endif