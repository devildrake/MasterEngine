#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>
#include <string>
#include <assimp/material.h>
class Material {
private:
	unsigned id;
	std::pair<int, int>texSize;
	std::string textureName;
	std::string texturePath;
public:
	Material(std::string name, std::string path, unsigned, std::pair<int, int>);
	Material(aiMaterial* mat, aiString file, std::string& materialPath, std::string modelPath);
	Material(aiMaterial* mat, std::string modelPath);
	~Material();
	const unsigned GetTextureID()const;
	const std::pair<int, int> Material::GetTextureSize()const;
	const std::string Material::GetTextureName()const;
	const std::string Material::GetTexturePath()const;

};

#endif