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
class ComponentCamera;
class GameObject;

class ModuleEditorCamera : public Module {
private:
	GameObject* gameObject;
	ComponentTransform* transform;
	ComponentCamera* camera;
public:

	ModuleEditorCamera(float3 aPos = float3::zero);
	~ModuleEditorCamera();

	bool Init() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;
	void MainWindowResized(unsigned, unsigned) override;
	//void SceneWindowResized(unsigned, unsigned)override;
	void SendViewModelMatrix();
	void SendProjectionMatrix();
	void SetFrustumPos(float3 newPos);
	Frustum& GetFrustum();
	const float GetNearPlane()const;
	const float GetFarPlane()const;
	void SetNearPlane(float);
	void SetFarPlane(float);
	void SetAspectRatio(float);
	void FocusOn(ComponentTransform* m, float focusDistance = 1.0f);
	void SetTargetGameObject(GameObject* m);

private:
	friend class ConfigWindow;
	Frustum* frustum;
	float pitch, yaw;
	float focusDistance;
	void* context;
	SDL_GLContext glcontext;

	float aspectRatio;
	float cameraSpeed, rotationSpeed, orbitSpeed;
	float zoomSpeed;
	//float3 frustumPosition;
	float nearPlaneDistance, farPlaneDistance;

	ComponentTransform* focusTarget = nullptr;
	float3 focusPosition;
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