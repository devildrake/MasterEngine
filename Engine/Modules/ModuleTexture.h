#ifndef _MODULE_TEXTURE_H_
#define _MODULE_TEXTURE_H_
#include "Module.h"
#include "../Application.h"
#include <IL/il.h>
//#include <IL/ilut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_main.h>
#include <gl/GL.h>
#include <map>
class ModuleTexture :public Module {

public:
	ILuint texid; /* ILuint is a 32bit unsigned integer.
	   Variable texid will be used to store image name. */
	ILboolean success; /* ILboolean is type similar to GLboolean and can equal GL_FALSE (0) or GL_TRUE (1)
	  it can have different value (because it's just typedef of unsigned char), but this sould be
	  avoided.
	  Variable success will be used to determine if some function returned success or failure. */
	GLuint image;
	int finished;

	ModuleTexture();
	~ModuleTexture();

	bool            Init();
	update_status   PreUpdate();
	update_status   Update();
	update_status   PostUpdate();
	bool            CleanUp();
	GLuint LoadTexture(const char* path);
	std::map < const char*, GLuint> textureMap;
private:

	//static DDRenderInterfaceCoreGL* implementation;
};



#endif