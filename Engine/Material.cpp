#include "Material.h"

Material::Material(std::string name, std::string path, unsigned id, std::pair<int, int> texSize) :id(id), texSize(texSize), textureName(name), texturePath(path) {
}
Material::~Material() {
}
const unsigned Material::GetTextureID()const {
	return id;
}

const std::pair<int, int> Material::GetTextureSize()const {
	return texSize;
}

const std::string Material::GetTextureName()const {
	return textureName;
}


const std::string Material::GetTexturePath()const {
	return texturePath;
}