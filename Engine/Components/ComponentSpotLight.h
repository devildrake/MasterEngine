#ifndef COMPONENT_SPOT_LIGHT_H
#define COMPONENT_SPOT_LIGHT_H
#include "ComponentLight.h"
class ComponentSpotLight :public ComponentLight {
private:
	float innerAngle, outerAngle;

public:
	ComponentSpotLight();
	~ComponentSpotLight();
};
#endif

