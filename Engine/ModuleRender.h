#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <glew.h>
#include "ModuleEditorCamera.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	float3 bgColor;
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	SDL_GLContext GetContext();
private:
	void* context;
	SDL_GLContext glcontext;

};
