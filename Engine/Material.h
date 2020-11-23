#ifndef MATERIAL_H
#define MATERIAL_H
#include <vector>

class Material {
private:
	unsigned id;
	std::pair<int, int>texSize;
public:
	Material(unsigned, std::pair<int, int>);
	~Material();
	const unsigned GetTextureID()const;
	const std::pair<int, int> Material::GetTextureSize()const;

};

#endif