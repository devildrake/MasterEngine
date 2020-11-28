#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>
#include <string>
class Material {
private:
	unsigned id;
	std::pair<int, int>texSize;
	std::string textureName;
	std::string texturePath;
public:
	Material(std::string, std::string, unsigned, std::pair<int, int>);
	~Material();
	const unsigned GetTextureID()const;
	const std::pair<int, int> Material::GetTextureSize()const;
	const std::string Material::GetTextureName()const;
	const std::string Material::GetTexturePath()const;

};

#endif