#ifndef _MODULE_TEXTURE_H_
#define _MODULE_TEXTURE_H_
#include "Module.h"
#include <glew.h>
#include <map>
#include <string>
#include "../MathGeoLib/Math/float2.h"

class ModuleTextures :public Module {
private:
	std::map < std::string, GLuint> textureMap;
	GLenum wrapMode, minFilter, magFilter;
	std::string texturesFolderName;
private:
	const bool GenTexture(std::string path, GLuint& newTextureID);
public:

	ModuleTextures();
	~ModuleTextures();
	void ReleaseTexture(std::string path);
	bool Init() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;
	const bool LoadTexture(std::string path, GLuint& tex, float2& texSize);
	const bool LoadTexture(std::string path, GLuint& tex);
	const bool LoadDevilImage(std::string path, GLuint& img);
	unsigned int LoadCubeMap(std::string files[6]);
	void ReleaseCubeMap(unsigned int id);
	void SetWrapMode(GLenum anEnum);
	void SetMinMode(GLenum anEnum);
	void SetMagMode(GLenum anEnum);
	std::string GetTexturesFolderName();
	GLenum GetWrapMode();
	GLenum GetMinFilter();
	GLenum GetMagFilter();
};



#endif