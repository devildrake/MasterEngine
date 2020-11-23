#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "../Model.h"
#include "../Leaks.h"
#define DEGTORAD 3.14159/180

ModuleEditorCamera::ModuleEditorCamera()
{
	nearPlaneDistance = 0.1f;
	farPlaneDistance = 200.0f;
	frustumPosition = float3(0, 1, -2);
	cameraSpeed = 6;
	rotationSpeed = 15;
	pitch = 0;
	yaw = 0;
	screenMargin = 20.0f;
	zoomSpeed = 10;
	frustumCulling = true;
	focusDistance = 2.0f;
	orbitSpeed = 6.0f;
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{
}

// Called before render is available
bool ModuleEditorCamera::Init()
{
	aspectRatio = (float)App->window->GetWidth() / (float)App->window->GetHeight();

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	SetAspectRatio(aspectRatio);
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
	yaw -= mouseMotion.x * rotationSpeed * App->GetDeltaTime();
	//yaw = math::Mod(yaw, 360.0f);

	return yaw - prevYaw;
}

const float ModuleEditorCamera::UpdateCameraPitch(const float3 mouseMotion) {

	float prevPitch = pitch;
	pitch -= mouseMotion.y * rotationSpeed * App->GetDeltaTime();
	pitch = math::Max(math::Min(pitch, 90.0f), -90.0f);
	return pitch - prevPitch;
}

const bool ModuleEditorCamera::WarpMouseTooCloseToEdges(float3 mousePos, float screenMargin)const {
	bool val = false;
	if (mousePos.x >= App->window->GetWidth() - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, screenMargin, mousePos.y);
		mousePos.x = screenMargin;
		val = true;
	}
	else if (mousePos.x <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, App->window->GetWidth() - screenMargin, mousePos.y);
		mousePos.x = App->window->GetWidth() - screenMargin;
		val = true;
	}
	if (mousePos.y >= App->window->GetHeight() - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, screenMargin);
		val = true;
	}
	else if (mousePos.y <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, App->window->GetHeight() - screenMargin);
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

const float ModuleEditorCamera::GetDistanceBasedOnBoundingBox(Model* m, float distanceFactor)const {
	float greatestDistance = 0;

	float xBounds = math::Abs(m->BoundingBox().second.x - m->BoundingBox().first.x);
	float yBounds = math::Abs(m->BoundingBox().second.y - m->BoundingBox().first.y);
	float zBounds = math::Abs(m->BoundingBox().second.z - m->BoundingBox().first.z);

	greatestDistance = math::Max(xBounds, yBounds, zBounds);

	return greatestDistance * distanceFactor;
}

void ModuleEditorCamera::FocusOn(Model* m, float focusDistance) {
	targetModel = m;
	float3 boundingBoxCenter = m->GetBoundingCenter();
	float3 focusPosition = boundingBoxCenter.Mul(targetModel->Scale()) + targetModel->Position();

	float3 newVecToTarget = focusPosition - frustumPosition;

	frustumPosition = focusPosition - newVecToTarget.Normalized() * GetDistanceBasedOnBoundingBox(m, focusDistance);

	float3x3 lookAtMat = float3x3::LookAt(frustum.Front(), newVecToTarget.Normalized(), frustum.Up(), float3::unitY);
	frustum.SetFront((lookAtMat * frustum.Front()).Normalized());
	frustum.SetUp(lookAtMat * frustum.Up());
}

// Called every draw update
update_status ModuleEditorCamera::Update()
{
	bool showCursor = true;
	float3 cameraMovementInput = float3(0, 0, 0);

	float speedFactor = App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT ? cameraSpeed * 3 : cameraSpeed;
	const float3 mouseMotion = App->input->GetMouseMotion();

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

					float3 mouseMotion = App->input->GetMouseMotion();

					float3 boundingBoxCenter = targetModel->GetBoundingCenter();
					float3 focusPosition = boundingBoxCenter.Mul(targetModel->Scale()) + targetModel->Position();

					float orbitDistance = focusPosition.Distance(frustumPosition);

					frustumPosition += frustum.WorldRight() * mouseMotion.x * App->GetDeltaTime() * orbitSpeed;
					frustumPosition += frustum.Up() * mouseMotion.y * App->GetDeltaTime() * orbitSpeed;

					float3 newVecToItem = (focusPosition - frustumPosition).Normalized();

					frustumPosition = focusPosition - newVecToItem * orbitDistance;

					newVecToItem = (focusPosition - frustumPosition).Normalized();

					float3x3 lookAtMat = float3x3::LookAt(frustum.Front(), newVecToItem, frustum.Up(), float3::unitY);

					frustum.SetFront(lookAtMat * frustum.Front());
					frustum.SetUp(lookAtMat * frustum.Up());


				}
			}

			showCursor = false;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			float totalMotion = mouseMotion.x + mouseMotion.y;
			frustumPosition += frustum.Front() * totalMotion * speedFactor * App->GetDeltaTime();
			showCursor = false;
		}
	}

	else {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			showCursor = false;
			ApplyUpdatedPitchYawToFrustum();

			//if (WarpMouseTooCloseToEdges(App->input->GetMousePosition(), screenMargin)) {
			//	App->input->ResetMouseMotion();
			//}

			frustumPosition += GetCameraMovementInput() * speedFactor * App->GetDeltaTime();
		}

		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			if (!App->input->IsMouseOverImGuiWindow()) {
				const float3 mouseMotion = App->input->GetMouseMotion();
				cameraMovementInput = -frustum.WorldRight() * mouseMotion.x * rotationSpeed * App->GetDeltaTime();
				cameraMovementInput += frustum.Up() * mouseMotion.y * rotationSpeed * App->GetDeltaTime();

				//if (WarpMouseTooCloseToEdges(App->input->GetMousePosition(), screenMargin)) {
				//	App->input->ResetMouseMotion();
				//}

				frustumPosition += cameraMovementInput * speedFactor * App->GetDeltaTime();
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (targetModel != nullptr) {
			FocusOn(targetModel, focusDistance);
		}
	}

	SDL_ShowCursor(showCursor);
	const float mouseWheelMotion = App->input->GetMouseWheelMotion();
	if (mouseWheelMotion != 0 && !App->input->IsMouseOverImGuiWindow()) {
		frustumPosition += mouseWheelMotion * frustum.Front() * zoomSpeed * App->GetDeltaTime();
	}

	frustum.SetPos(frustumPosition);
	SendViewModelMatrix();
	SendProjectionMatrix();
	return UPDATE_CONTINUE;
}

void ModuleEditorCamera::SetTargetModel(Model* m) {
	FocusOn(m, focusDistance);
}


update_status ModuleEditorCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp()
{
	return true;
}

void ModuleEditorCamera::WindowResized(unsigned width, unsigned height)
{
	SetAspectRatio((float)width / (float)height);
}

void ModuleEditorCamera::SetFrustumPos(float3 newPos) {
	frustumPosition = newPos;
	frustum.SetPos(newPos);
}

Frustum* ModuleEditorCamera::GetFrustum() {
	return &frustum;
}

const float ModuleEditorCamera::GetNearPlane()const {
	return nearPlaneDistance;
}
const float ModuleEditorCamera::GetFarPlane()const {
	return farPlaneDistance;
}
void ModuleEditorCamera::SetNearPlane(float n) {
	nearPlaneDistance = n;
	frustum.SetViewPlaneDistances(n, farPlaneDistance);
}
void ModuleEditorCamera::SetFarPlane(float n) {
	farPlaneDistance = n;
	frustum.SetViewPlaneDistances(nearPlaneDistance, n);
}

void ModuleEditorCamera::SetAspectRatio(float n) {
	aspectRatio = n;
	frustum.SetHorizontalFovAndAspectRatio((DEGTORAD * 90), n);
}