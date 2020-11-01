#ifndef MODULE_EDITOR_CAMERA_H
#define MODULE_EDITOR_CAMERA_H
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <glew.h>
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3x3.h"
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
	float UpdateCameraYaw();
	float UpdateCameraPitch();
	float3 GetCameraMovementInput()const;
	void SendViewModelMatrix();
	void SendProjectionMatrix();

private:

	Frustum frustum;
	float3 frustumPosition;
	float3 originalFront, originalUp;
	float cameraSpeed, mouseSensitivity;
	float pitch, yaw;
	float nearPlaneDistance, farPlaneDistance;
	void* context;
	SDL_GLContext glcontext;
};
#endif