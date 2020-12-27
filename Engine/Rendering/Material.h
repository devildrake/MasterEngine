#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>
#include <string>
#include <assimp/material.h>
#include "../MathGeoLib/Math/float2.h"
class Material {
private:
	unsigned id;
	float2 texSize;
	std::string textureName;
	std::string texturePath;
public:
	//Material(std::string name, std::string path, unsigned, float2);
	//Material(aiMaterial* mat, aiString file, std::string& materialPath, std::string modelPath);
	Material(aiMaterial* mat, std::string modelPath);
	~Material();
	const unsigned GetTextureID()const;
	const float2 Material::GetTextureSize()const;
	const std::string& Material::GetTextureName()const;
	const std::string& Material::GetTexturePath()const;

};

#endif