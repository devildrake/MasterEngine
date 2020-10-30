#ifndef MODULE_EDITOR_CAMERA_H
#define MODULE_EDITOR_CAMERA_H
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <glew.h>
#include "MathGeoLib/Geometry/Frustum.h"
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	float4x4 GetTransposedProjectionMatrix();
	float4x4 GetTransposedViewModelMatrix();

	void SendViewModelMatrix();
	void SendProjectionMatrix();

private:
	Frustum frustum;
	void* context;
	SDL_GLContext glcontext;

};
#endif