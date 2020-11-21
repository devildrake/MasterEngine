#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H
#include "Module.h"
#include "SDL.h"
#include "../Utilities/Globals.h"
#include "../MathGeoLib/Math/float3.h"
#include <list>
class Shader;
class Model;

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
	void AddModel(Model* m);
	void RemoveModel(Model* m);
	//bool AddModel(Model* model);
	//bool RemoveModel(Model* model);

private:

	void* context;
	float3 bgColor;
	std::list<Model*> models;
	SDL_GLContext glcontext;
	Shader* default_shader;
};
#endif