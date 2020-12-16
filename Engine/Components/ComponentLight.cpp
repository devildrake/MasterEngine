#include "ComponentLight.h"
#include "../GameObject.h"

ComponentLight::ComponentLight(GameObject* go, LightType type) :Component(ComponentType::CTLight, go), type(type) {

}

ComponentLight::~ComponentLight() {

}
