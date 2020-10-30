#include "Globals.h"
#include "Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#define DEGTORAD 3.14159/180

ModuleEditorCamera::ModuleEditorCamera()
{
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{
}

// Called before render is available
bool ModuleEditorCamera::Init()
{

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);
	frustum.SetPos(float3(0, 1, -2));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);


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

// Called every draw update
update_status ModuleEditorCamera::Update()
{

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
}

