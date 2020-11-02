#include "Globals.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#define DEGTORAD 3.14159/180

ModuleEditorCamera::ModuleEditorCamera()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	nearPlaneDistance = 0.1f;
	farPlaneDistance = 200.0f;

	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);
	frustumPosition = float3(0, 1, -2);

	frustum.SetPos(frustumPosition);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	originalFront = frustum.Front();
	originalUp = frustum.Up();
	cameraSpeed = 6;
	mouseSensitivity = 15;
	pitch = 0;
	yaw = 0;

}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{
}

// Called before render is available
bool ModuleEditorCamera::Init()
{





	return true;
}

update_status ModuleEditorCamera::PreUpdate()
{


	return UPDATE_CONTINUE;
}


float4x4 ModuleEditorCamera::GetTransposedProjectionMatrix() {
	return frustum.ProjectionMatrix().Transposed();
}

float4x4 ModuleEditorCamera::GetTransposedViewModelMatrix() {
	float4x4 viewMatrix = frustum.ViewMatrix();
	return viewMatrix.Transposed();
}

void ModuleEditorCamera::SendViewModelMatrix() {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*(GetTransposedViewModelMatrix()).v);
}

void ModuleEditorCamera::SendProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*GetTransposedProjectionMatrix().v);
}

float3 ModuleEditorCamera::GetCameraMovementInput() const {
	float3 val = float3(0, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		val += frustum.Front();
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		val -= frustum.Front();
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		val -= frustum.WorldRight();
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		val += frustum.WorldRight();
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		val += frustum.Up();
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		val -= frustum.Up();
	}
	return val;
}

float ModuleEditorCamera::UpdateCameraYaw() {
	yaw -= App->input->GetMouseMotion().x * mouseSensitivity * App->GetDeltaTime();
	yaw = math::Mod(yaw, 360.0f);
	return yaw;
}

float ModuleEditorCamera::UpdateCameraPitch() {
	pitch += App->input->GetMouseMotion().y * mouseSensitivity * App->GetDeltaTime();
	pitch = math::Max(math::Min(pitch, 90.0f), -90.0f);
	return pitch;
}

// Called every draw update
update_status ModuleEditorCamera::Update()
{
	SDL_ShowCursor(1);


	float3 cameraMovementInput = float3(0, 0, 0);
	float speedFactor = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT ? 6 : 3;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

		cameraMovementInput = GetCameraMovementInput();

		float3x3 yawRotation = float3x3::RotateAxisAngle(float3(0, 1, 0), DEGTORAD * UpdateCameraYaw());
		float3x3 pitchRotation = float3x3::RotateAxisAngle(float3(1, 0, 0), DEGTORAD * UpdateCameraPitch());

		float3 newFront = yawRotation * pitchRotation * originalFront;
		float3 newUp = yawRotation * pitchRotation * originalUp;
		frustum.SetFront(newFront);
		frustum.SetUp(newUp);

		frustumPosition += cameraMovementInput * speedFactor * App->GetDeltaTime();
	}

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		if (!(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) {
			cameraMovementInput = -frustum.WorldRight() * App->input->GetMouseMotion().x * mouseSensitivity * App->GetDeltaTime();
			cameraMovementInput += frustum.Up() * App->input->GetMouseMotion().y * mouseSensitivity * App->GetDeltaTime();


			frustumPosition += cameraMovementInput * speedFactor * App->GetDeltaTime();
		}
		else {
			//TO DO Implement orbitMode
		}

	}

	if (App->input->GetMouseWheelMotion() != 0) {
		frustumPosition += App->input->GetMouseWheelMotion() * frustum.Front() * speedFactor * 5 * App->GetDeltaTime();
	}

	frustum.SetPos(frustumPosition);
	SendViewModelMatrix();
	SendProjectionMatrix();
	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate()
{
	//SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleEditorCamera::WindowResized(unsigned width, unsigned height)
{
	//frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * 90.0f), width / height);
	float newRatio = width / height;
	frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * (newRatio * 90 / 1.3)), newRatio);
}

