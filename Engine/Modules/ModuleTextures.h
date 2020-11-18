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

public:

	ModuleTextures();
	~ModuleTextures(); 

	bool            Init();
	update_status   PreUpdate();
	update_status   Update();
	update_status   PostUpdate();
	bool            CleanUp();
	GLuint LoadTexture(std::string path);
	void SetWrapMode(GLenum anEnum);
	void SetMinMode(GLenum anEnum);
	void SetMagMode(GLenum anEnum);	
	
	GLenum GetWrapMode();
	GLenum GetMinFilter();
	GLenum GetMagFilter();
};



#endif