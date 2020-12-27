#include "ComponentPointLight.h"
#include "ComponentTransform.h"
#include "../GameObject.h"
#include "../ImGui/imgui.h"
#include "../Application.h"
#include "../Modules/ModuleRender.h"
#include <glew.h>

ComponentPointLight::ComponentPointLight(GameObject* anOwner, float3 pos, float anInt, float3 aColor, float cAtt, float lAtt, float qAtt) :ComponentLight(anOwner, ComponentLight::LightType::POINT, aColor, anInt), constantAtt(cAtt), linearAtt(lAtt), quadraticAtt(qAtt) {
	if (owner != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)owner->GetComponentOfType(ComponentType::CTTransformation);
		if (transform != nullptr) {
			transform->SetPosition(pos);
		} else {
			transform = (ComponentTransform*)owner->CreateComponent(ComponentType::CTTransformation);
			transform->SetPosition(pos);
		}
	}
}

ComponentPointLight::~ComponentPointLight() {

}

void ComponentPointLight::DrawEditor() {
	if (ImGui::CollapsingHeader("Point Light")) {
		ComponentLight::DrawEditor();

		if (ImGui::InputFloat("Constant Attenuation", &constantAtt)) {}
		if (ImGui::InputFloat("Lienar Attenuation", &constantAtt)) {}
		if (ImGui::InputFloat("Quadratic Attenuation", &constantAtt)) {}

	}
}

void ComponentPointLight::DrawGizmos() {

}
