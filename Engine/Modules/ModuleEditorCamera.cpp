#include <Globals.h>
#include "../Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "../ImGuiWindows/SceneWindow.h"
#include "../GameObject.h"
#include "../Components/ComponentTransform.h"
#include "../Components/ComponentCamera.h"
#include "../Components/ComponentMeshRenderer.h"
//#include "../Rendering/Model.h"
#include <Leaks.h>

ModuleEditorCamera::ModuleEditorCamera(float3 aPos) :Module("Editor Camera"),
cameraSpeed(6), rotationSpeed(15), pitch(0), yaw(0), zoomSpeed(10), focusDistance(2.0f), orbitSpeed(20.0f), context(nullptr), glcontext(nullptr) {

	gameObject = new GameObject("EditorCamera");
	transform = (ComponentTransform*)gameObject->CreateComponent(Component::ComponentType::CTTransformation);
	transform->localPosition = aPos;
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera() {
	delete gameObject;
}

// Called before render is available
bool ModuleEditorCamera::Init() {
	camera = (ComponentCamera*)gameObject->CreateComponent(Component::ComponentType::CTCamera);

	return true;
}

UpdateStatus ModuleEditorCamera::PreUpdate() {
	return UPDATE_CONTINUE;
}


const float4x4 ModuleEditorCamera::GetTransposedProjectionMatrix() const {
	return camera->GetFrustum().ProjectionMatrix().Transposed();
}

const float4x4 ModuleEditorCamera::GetTransposedViewModelMatrix()const {
	float4x4 viewMatrix = camera->GetFrustum().ViewMatrix();
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

	if (App->input->GetKey(SDL_SCANCODE_W) == ModuleInput::KEY_REPEAT) {
		val += camera->GetFrustum().Front();
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == ModuleInput::KEY_REPEAT) {
		val -= camera->GetFrustum().Front();
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == ModuleInput::KEY_REPEAT) {
		val -= camera->GetFrustum().WorldRight();
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == ModuleInput::KEY_REPEAT) {
		val += camera->GetFrustum().WorldRight();
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == ModuleInput::KEY_REPEAT) {
		val += camera->GetFrustum().Up();
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == ModuleInput::KEY_REPEAT) {
		val -= camera->GetFrustum().Up();
	}
	return val;
}

const float ModuleEditorCamera::UpdateCameraYaw(const float3 mouseMotion) {

	float prevYaw = yaw;
	yaw -= mouseMotion.x * rotationSpeed * App->GetDeltaTime();
	yaw = math::Mod(yaw, 360.0f);
	return yaw - prevYaw;
}

const float ModuleEditorCamera::UpdateCameraPitch(const float3 mouseMotion) {

	float prevPitch = pitch;
	pitch -= mouseMotion.y * rotationSpeed * App->GetDeltaTime();
	pitch = math::Max(math::Min(pitch, 90.0f), -90.0f);
	return pitch - prevPitch;
}

void ModuleEditorCamera::ApplyUpdatedPitchYawToFrustum() {
	const float3 mouseMotion = App->input->GetMouseMotion();

	float relativeYaw = UpdateCameraYaw(mouseMotion);
	float relativePitch = UpdateCameraPitch(mouseMotion);

	float3x3 yawRotation = float3x3::RotateAxisAngle(float3(0, 1, 0), DEGTORAD * relativeYaw);
	float3x3 pitchRotation = float3x3::RotateAxisAngle(camera->GetFrustum().WorldRight().Normalized(), DEGTORAD * relativePitch);

	float3 newUp = yawRotation * pitchRotation * camera->GetFrustum().Up().Normalized();
	float3 newFront = yawRotation * pitchRotation * camera->GetFrustum().Front().Normalized();

	camera->GetFrustum().SetUp(newUp.Normalized());
	camera->GetFrustum().SetFront(newFront.Normalized());
}

//FocusPosition is recalculated here, frustum is also repositioned so that targeted AABB (if found) fits inside frustum
void ModuleEditorCamera::FocusOn(ComponentTransform* m, float focusDistance) {
	std::vector<Component*>meshRenderers = m->owner->GetComponentsInChildrenOfType(Component::ComponentType::CTMeshRenderer);
	if (meshRenderers.size() > 0) {
		AABB generalAABB;

		float3 absoluteMax = math::vec(-10000, -10000, -10000);
		float3 absoluteMin = float3(100000, 100000, 10000);

		for (std::vector<Component*>::const_iterator it = meshRenderers.begin(); it != meshRenderers.end(); ++it) {
			AABB meshAABB = ((ComponentMeshRenderer*)(*it))->GetAABB();

			if (meshAABB.minPoint.x < absoluteMin.x) {
				absoluteMin.x = meshAABB.minPoint.x;
			}
			if (meshAABB.minPoint.y < absoluteMin.y) {
				absoluteMin.y = meshAABB.minPoint.y;
			}
			if (meshAABB.minPoint.z < absoluteMin.z) {
				absoluteMin.z = meshAABB.minPoint.z;
			}

			if (meshAABB.maxPoint.x > absoluteMax.x) {
				absoluteMax.x = meshAABB.maxPoint.x;
			}
			if (meshAABB.maxPoint.y > absoluteMax.y) {
				absoluteMax.y = meshAABB.maxPoint.y;
			}
			if (meshAABB.maxPoint.z > absoluteMax.z) {
				absoluteMax.z = meshAABB.maxPoint.z;
			}

			generalAABB.minPoint = absoluteMin;
			generalAABB.maxPoint = absoluteMax;

			focusPosition = generalAABB.CenterPoint();
			transform->localPosition = focusPosition;
			camera->GetFrustum().SetPos(transform->localPosition);

			//Dangerous while over here. Big distances may cause problems, emergency exit at 1000 iterations may be required(?)
			while (!camera->GetFrustum().Contains(generalAABB)) {
				transform->localPosition -= camera->GetFrustum().Front() * 10;
				camera->GetFrustum().SetPos(transform->localPosition);
			}
		}
	}
}
//
//void ModuleEditorCamera::FocusOn_Old(ComponentTransform* m, float focusDistance) {
//
//	ComponentMeshRenderer* meshRenderer = ((ComponentMeshRenderer*)m->owner->GetComponentOfType(Component::ComponentType::CTMeshRenderer));
//
//	if (meshRenderer != nullptr) {
//
//		focusPosition = meshRenderer->GetAABB().CenterPoint();
//		transform->localPosition = focusPosition;
//		camera->GetFrustum()->SetPos(transform->localPosition);
//
//		//Dangerous while over here. Big distances may cause problems, emergency exit at 1000 iterations may be required(?)
//		while (!camera->GetFrustum()->Contains(meshRenderer->GetAABB())) {
//			transform->localPosition -= camera->GetFrustum()->Front() * 10;
//			camera->GetFrustum()->SetPos(transform->localPosition);
//		}
//	} else {
//		focusPosition = m->CalculateGlobalPosition();
//		transform->localPosition = m->CalculateGlobalPosition() - camera->GetFrustum()->Front() * focusDistance;
//	}
//}

// Called every draw update
UpdateStatus ModuleEditorCamera::Update() {
	//Camera movement inputs will not be processed if the mouse is not over the scene editor view
	if (!App->editor->GetScene()->IsHovered())return UPDATE_CONTINUE;
	bool showCursor = true;
	float3 cameraMovementInput = float3(0, 0, 0);

	float speedFactor = App->input->GetKey(SDL_SCANCODE_LSHIFT) == ModuleInput::KEY_REPEAT ? cameraSpeed * 3 : cameraSpeed;
	const float3 mouseMotion = App->input->GetMouseMotion();

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == ModuleInput::KEY_REPEAT)) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == ModuleInput::KEY_REPEAT) {

			//Push camera position temporarily to focus position
			transform->localPosition -= focusPosition;

			//Generate rotation matrices
			float newYaw = mouseMotion.x * App->GetDeltaTime() * orbitSpeed;
			float newPitch = -mouseMotion.y * App->GetDeltaTime() * orbitSpeed;

			float3x3 rotMatYaw = float3x3::RotateAxisAngle(camera->GetFrustum().Up(), DEGTORAD * newYaw);
			float3x3 rotMatPitch = float3x3::RotateAxisAngle(camera->GetFrustum().WorldRight(), DEGTORAD * newPitch);

			//Rotate camera frustum with focus position as origin
			transform->localPosition = rotMatPitch * rotMatYaw * transform->localPosition;


			//Pop camera position back from focusPosition
			transform->localPosition += focusPosition;

			//Calculate Look at matrix
			float3 newVecToItem = (focusPosition - transform->localPosition);
			newVecToItem.Normalize();

			float3x3 lookAtMat = float3x3::LookAt(camera->GetFrustum().Front(), newVecToItem, camera->GetFrustum().Up(), float3::unitY);

			//Apply Look at matrix
			camera->GetFrustum().SetFront((lookAtMat * camera->GetFrustum().Front()).Normalized());
			camera->GetFrustum().SetUp((lookAtMat * camera->GetFrustum().Up()).Normalized());

			showCursor = false;
		} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == ModuleInput::KEY_REPEAT) {
			float totalMotion = mouseMotion.x + mouseMotion.y;
			transform->localPosition += camera->GetFrustum().Front() * totalMotion * speedFactor * App->GetDeltaTime();
			showCursor = false;
		}
	}
	//ALT NOT PRESSED
	else {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == ModuleInput::KEY_REPEAT) {
			showCursor = false;
			ApplyUpdatedPitchYawToFrustum();

			transform->localPosition += GetCameraMovementInput() * speedFactor * App->GetDeltaTime();
		}

		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == ModuleInput::KEY_REPEAT) {



			if (App->editor->GetScene()->IsMouseOverWindow()) {
				const float3 mouseMotion = App->input->GetMouseMotion();
				cameraMovementInput = -camera->GetFrustum().WorldRight() * mouseMotion.x * rotationSpeed * App->GetDeltaTime();
				cameraMovementInput += camera->GetFrustum().Up() * mouseMotion.y * rotationSpeed * App->GetDeltaTime();

				transform->localPosition += cameraMovementInput * speedFactor * App->GetDeltaTime();
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == ModuleInput::KEY_DOWN) {
		if (focusTarget != nullptr) {
			FocusOn(focusTarget, focusDistance);
		}
	}

	SDL_ShowCursor(showCursor);

	const float mouseWheelMotion = App->input->GetMouseWheelMotion();
	//if (mouseWheelMotion != 0 && !App->input->IsMouseOverImGuiWindow()) {
	transform->localPosition += mouseWheelMotion * camera->GetFrustum().Front() * zoomSpeed * App->GetDeltaTime();
	//}

	camera->GetFrustum().SetPos(transform->localPosition);
	SendViewModelMatrix();
	SendProjectionMatrix();
	return UPDATE_CONTINUE;
}

void ModuleEditorCamera::SetTargetGameObject(GameObject* m) {
	if (m == nullptr) {
		focusTarget = nullptr;
		return;
	}

	ComponentTransform* transform = (ComponentTransform*)m->GetComponentOfType(Component::ComponentType::CTTransformation);
	if (transform != nullptr) {
		focusTarget = transform;
	}
	/*if (transform != nullptr) {
		FocusOn(transform, focusDistance);
	}*/

}


UpdateStatus ModuleEditorCamera::PostUpdate() {
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp() {
	return true;
}

//void ModuleEditorCamera::SceneWindowResized(unsigned width, unsigned height) {
//	SetAspectRatio((float)width / (float)height);
//}

void ModuleEditorCamera::MainWindowResized(unsigned width, unsigned height) {
	//SetAspectRatio((float)width / (float)height);
}

void ModuleEditorCamera::SetFrustumPos(float3 newPos) {
	transform->localPosition = newPos;
	camera->GetFrustum().SetPos(newPos);
}

Frustum& ModuleEditorCamera::GetFrustum() {
	return camera->GetFrustum();
}



const float ModuleEditorCamera::GetNearPlane()const {
	return camera->GetFrustum().NearPlaneDistance();
}
const float ModuleEditorCamera::GetFarPlane()const {
	return camera->GetFrustum().FarPlaneDistance();
}
void ModuleEditorCamera::SetNearPlane(float n) {
	//nearPlaneDistance = n;
	camera->GetFrustum().SetViewPlaneDistances(n, camera->GetFrustum().FarPlaneDistance());
}
void ModuleEditorCamera::SetFarPlane(float n) {
	//farPlaneDistance = n;
	camera->GetFrustum().SetViewPlaneDistances(camera->GetFrustum().NearPlaneDistance(), n);
}

void ModuleEditorCamera::SetAspectRatio(float n) {
	//aspectRatio = n;

	//LOG("new aspect ratio %f", aspectRatio);
	//frustum->SetHorizontalFovAndAspectRatio(DegToRad(90), n);
	camera->GetFrustum().SetHorizontalFovAndAspectRatio(DegToRad(90), n);
}