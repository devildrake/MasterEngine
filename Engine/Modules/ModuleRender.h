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
	const unsigned GetDefaultShaderID()const;
	friend class ConfigWindow;

	//bool AddModel(Model* model);
	//bool RemoveModel(Model* model);

private:
	void* context;
	float3 bgColor;
	std::vector<Model*> models;
	SDL_GLContext glcontext;
	Shader* default_shader;
};
