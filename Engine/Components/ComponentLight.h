#ifndef COMPONENT_LIGHT_H_
#define COMPONENT_LIGHT_H_
#include "Component.h"
#include "../MathGeoLib/Math/float3.h"
#include "../Rendering/Shader.h"


class ComponentLight :public Component {

protected:
	float3 lightColor;
	float lightIntensity;
public:
	enum LightType { DIRECTIONAL, POINT, SPOT };
private:
	LightType type;
public:
	ComponentLight(GameObject* go, LightType type = LightType::DIRECTIONAL, float3 aColor = float3::zero, float anInt = 1.0f);
	~ComponentLight();
	void DrawEditor()override;
};

#endif
