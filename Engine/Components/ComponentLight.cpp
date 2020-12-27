#include "ComponentLight.h"
#include "../GameObject.h"
#include "../ImGui/imgui.h"
#include <glew.h>
ComponentLight::ComponentLight(GameObject* go, LightType type, float3 aColor, float anInt) :Component(ComponentType::CTLight, go), type(type), lightColor(aColor), lightIntensity(anInt) {

}

ComponentLight::~ComponentLight() {

}

void ComponentLight::DrawEditor() {
	ImGui::InputFloat("Intensity", &lightIntensity);
	ImGui::InputFloat3("Color", lightColor.ptr());
}
