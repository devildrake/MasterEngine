#ifndef MODULE_EDITOR_CAMERA_H
#define MODULE_EDITOR_CAMERA_H
#include "Module.h"
#include <Globals.h>
#include <SDL.h>
#include <glew.h>
#include "../MathGeoLib/Geometry/Frustum.h"
#include "../MathGeoLib/Math/float3x3.h"
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ComponentTransform;

class ModuleEditorCamera : public Module {
public:

	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	void SendViewModelMatrix();
	void SendProjectionMatrix();
	void SetFrustumPos(float3 newPos);
	Frustum* GetFrustum();
	const float GetNearPlane()const;
	const float GetFarPlane()const;
	void SetNearPlane(float);
	void SetFarPlane(float);
	void SetAspectRatio(float);
	void FocusOn(ComponentTransform* m, float);
	void SetTargetModel(ComponentTransform* m);

private:
	friend class ConfigWindow;
	Frustum frustum;
	float pitch, yaw;
	float focusDistance;
	void* context;
	SDL_GLContext glcontext;

	float aspectRatio;
	float cameraSpeed, rotationSpeed, orbitSpeed;
	float zoomSpeed;
	float3 frustumPosition;
	float nearPlaneDistance, farPlaneDistance;
	ComponentTransform* targetModel = nullptr;
private:
	const float4x4 GetTransposedProjectionMatrix()const;
	const float4x4 GetTransposedViewModelMatrix()const;
	const float UpdateCameraYaw(const float3 mouseMotion);
	const float UpdateCameraPitch(const float3 mouseMotion);
	const float3 GetCameraMovementInput()const;

	void ApplyUpdatedPitchYawToFrustum();
	//const float GetDistanceBasedOnBoundingBox(ComponentTransfom* m, float distanceFactor)const;

};
#endif