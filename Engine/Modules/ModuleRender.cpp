#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

ModuleRender::ModuleRender()
{
	bgColor = float3(0.2f, 0.2f, 0.2f);
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	//glewInit();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // desired version
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);



	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	return true;
}

update_status ModuleRender::PreUpdate()
{
	SDL_GetWindowSize(App->window->window,
		&App->window->screen_surface->w, &App->window->screen_surface->h);

	glViewport(0, 0, App->window->screen_surface->w, App->window->screen_surface->h);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
	//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}


// Called every draw update
update_status ModuleRender::Update()
{
	//Hardcoded grid drawing
	//Draws are called
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//glLineWidth(1.0f);
	//float d = 200.0f;
	//glBegin(GL_LINES);
	//for (float i = -d; i <= d; i += 1.0f)
	//{
	//	glVertex3f(i, 0.0f, -d);
	//	glVertex3f(i, 0.0f, d);
	//	glVertex3f(-d, 0.0f, i);
	//	glVertex3f(d, 0.0f, i);
	//}
	//glEnd();

	//glLineWidth(2.0f);
	//glBegin(GL_LINES);
	//// red X
	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	//glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	//// green Y
	//glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	//glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	//glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	//// blue Z
	//glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	//glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	//glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	//glEnd();
	//glLineWidth(1.0f);

	if (App->editorCamera != nullptr) {
		App->editorCamera->SendViewModelMatrix();
		App->editorCamera->SendProjectionMatrix();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

SDL_GLContext ModuleRender::GetContext() {
	return glcontext;
}
