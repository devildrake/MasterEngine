#pragma once
#include "Module.h"
#include "../Utilities/Globals.h"
#include "SDL.h"
#include "../MathGeoLib/Math/float3.h"
#include "../Model.h"
#include "../Shader.h"
//#include <glew.h>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:

	ModuleRender();
	~ModuleRender();
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	SDL_GLContext GetContext();
	friend class ModuleEditor;
	const unsigned GetDefaultShaderID()const;
private:
	void* context;
	float3 bgColor;

	SDL_GLContext glcontext;
	std::vector<Model*> models;
	Shader* default_shader;
};
