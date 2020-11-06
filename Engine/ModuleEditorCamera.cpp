#include "Globals.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#define DEGTORAD 3.14159/180

ModuleEditorCamera::ModuleEditorCamera()
{
	nearPlaneDistance = 0.1f;
	farPlaneDistance = 200.0f;
	frustumPosition = float3(0, 1, -2);
	cameraSpeed = 6;
	mouseSensitivity = 15;
	pitch = 0;
	yaw = 0;
	screenMargin = 20.0f;
	mouseWheelSpeedFactor = 5;
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{
}

// Called before render is available
bool ModuleEditorCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, SCREEN_WIDTH / SCREEN_HEIGHT);
	frustum.SetPos(frustumPosition);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

update_status ModuleEditorCamera::PreUpdate()
{


	return UPDATE_CONTINUE;
}


const float4x4 ModuleEditorCamera::GetTransposedProjectionMatrix() const {
	return frustum.ProjectionMatrix().Transposed();
}

const float4x4 ModuleEditorCamera::GetTransposedViewModelMatrix()const {
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

const float3 ModuleEditorCamera::GetCameraMovementInput() const {
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


const float ModuleEditorCamera::UpdateCameraYaw(const float3 mouseMotion) {

	float prevYaw = yaw;
	yaw -= mouseMotion.x * mouseSensitivity * App->GetDeltaTime();
	//yaw = math::Mod(yaw, 360.0f);

	return yaw - prevYaw;
}

const float ModuleEditorCamera::UpdateCameraPitch(const float3 mouseMotion) {

	float prevPitch = pitch;
	pitch -= mouseMotion.y * mouseSensitivity * App->GetDeltaTime();
	pitch = math::Max(math::Min(pitch, 90.0f), -90.0f);
	return pitch - prevPitch;
}

const bool ModuleEditorCamera::WarpMouseTooCloseToEdges(float3 mousePos, float screenMargin)const {
	bool val = false;
	if (mousePos.x >= SCREEN_WIDTH - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, screenMargin, mousePos.y);
		mousePos.x = screenMargin;
		val = true;
	}
	else if (mousePos.x <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, SCREEN_WIDTH - screenMargin, mousePos.y);
		mousePos.x = SCREEN_WIDTH - screenMargin;
		val = true;
	}
	if (mousePos.y >= SCREEN_HEIGHT - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, screenMargin);
		val = true;
	}
	else if (mousePos.y <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, SCREEN_HEIGHT - screenMargin);
		val = true;
	}
	return val;
}


void ModuleEditorCamera::ApplyUpdatedPitchYawToFrustum() {
	const float3 mouseMotion = App->input->GetMouseMotion();

	float relativeYaw = UpdateCameraYaw(mouseMotion);
	float relativePitch = UpdateCameraPitch(mouseMotion);

	float3x3 yawRotation = float3x3::RotateAxisAngle(float3(0, 1, 0), DEGTORAD * relativeYaw);
	float3x3 pitchRotation = float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), DEGTORAD * relativePitch);

	float3 newUp = yawRotation * pitchRotation * frustum.Up().Normalized();
	float3 newFront = yawRotation * pitchRotation * frustum.Front().Normalized();

	frustum.SetUp(newUp.Normalized());
	frustum.SetFront(newFront.Normalized());

}


// Called every draw update
update_status ModuleEditorCamera::Update()
{

	float3 cameraMovementInput = float3(0, 0, 0);
	float speedFactor = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT ? 6 : 3;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {

		ApplyUpdatedPitchYawToFrustum();

		//if (WarpMouseTooCloseToEdges(App->input->GetMousePosition(), screenMargin)) {
		//	App->input->ResetMouseMotion();
		//}

		frustumPosition += GetCameraMovementInput() * speedFactor * App->GetDeltaTime();
	}

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		if (!(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) {
			const float3 mouseMotion = App->input->GetMouseMotion();
			cameraMovementInput = -frustum.WorldRight() * mouseMotion.x * mouseSensitivity * App->GetDeltaTime();
			cameraMovementInput += frustum.Up() * mouseMotion.y * mouseSensitivity * App->GetDeltaTime();

			//if (WarpMouseTooCloseToEdges(App->input->GetMousePosition(), screenMargin)) {
			//	App->input->ResetMouseMotion();
			//}

			frustumPosition += cameraMovementInput * speedFactor * App->GetDeltaTime();
		}
		else {
			//TO DO Implement orbitMode
		}

	}
	SDL_ShowCursor(!(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT));
	const float mouseWheelMotion = App->input->GetMouseWheelMotion();
	if (mouseWheelMotion != 0) {
		frustumPosition += mouseWheelMotion * frustum.Front() * speedFactor * mouseWheelSpeedFactor * App->GetDeltaTime();
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
	return true;
}

void ModuleEditorCamera::WindowResized(unsigned width, unsigned height)
{
	//frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * 90.0f), width / height);
	float newRatio = width / height;
	frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * (newRatio * 90 / 1.3)), newRatio);
}



