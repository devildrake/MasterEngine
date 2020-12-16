#ifndef COMPONENT_LIGHT_H_
#define COMPONENT_LIGHT_H_
#include "Component.h"
#include "../MathGeoLib/Math/float3.h"
class ComponentLight :public Component {
protected:
	float3 lightColor;
public:
	enum LightType { DIRECTIONAL, POINT, FOCAL };
private:
	LightType type;
public:
	ComponentLight(GameObject* go, LightType type = LightType::DIRECTIONAL);
	~ComponentLight();
};

#endif
