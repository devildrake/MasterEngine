#include "Material.h"

Material::Material(unsigned id, std::pair<int, int> texSize) :id(id), texSize(texSize) {
}
Material::~Material() {
}
const unsigned Material::GetTextureID()const {
	return id;
}

const std::pair<int, int> Material::GetTextureSize()const {
	return texSize;
}