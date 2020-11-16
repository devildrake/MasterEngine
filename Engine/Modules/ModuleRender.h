#pragma once
#include "Module.h"
#include "../Utilities/Globals.h"
#include "SDL.h"
#include <glew.h>
#include "ModuleEditorCamera.h"
#include "../Model.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	float3 bgColor;
	unsigned default_program;
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




	char* LoadShaderSource(const char* shader_file_name);
	unsigned	CompileShader(unsigned type, const char* source);
	unsigned	CreateProgram(unsigned vtx_shader, unsigned frg_shader);

private:
	void* context;
	SDL_GLContext glcontext;
	std::vector<Model> models;

};
