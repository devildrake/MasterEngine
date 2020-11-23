#ifndef _MODULE_TEXTURE_H_
#define _MODULE_TEXTURE_H_
#include "Module.h"
#include "glew.h"
#include <map>
#include <string>

class ModuleTextures :public Module {
private:
	std::map < std::string, GLuint> textureMap;
	GLenum wrapMode, minFilter, magFilter;
	std::string texturesFolderName;
private:
	const bool GenTexture(std::string path, GLuint* newTextureID);
public:

	ModuleTextures();
	~ModuleTextures();
	void ReleaseTexture(std::string path);
	bool Init();
	update_status   PreUpdate();
	update_status   Update();
	update_status   PostUpdate();
	bool            CleanUp();
	const bool LoadTexture(std::string path, GLuint* tex);
	void SetWrapMode(GLenum anEnum);
	void SetMinMode(GLenum anEnum);
	void SetMagMode(GLenum anEnum);
	std::string GetTexturesFolderName();
	GLenum GetWrapMode();
	GLenum GetMinFilter();
	GLenum GetMagFilter();
};



#endif