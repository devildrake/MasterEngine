#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include <glew.h>

ModuleRender::ModuleRender()
{
	bgColor = float3(0.2f, 0.2f, 0.2f);
}

// Destructor
ModuleRender::~ModuleRender()
{
	if (default_shader != nullptr)
		delete default_shader;
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

bool ModuleRender::Start() {

	models.push_back(new Model("BakerHouse.fbx"));

	default_shader = new Shader("texturedModelVert.vs", "texturedModelFrag.fs");
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

const unsigned ModuleRender::GetDefaultShaderID() const {
	return default_shader->GetID();
}

// Called every draw update
update_status ModuleRender::Update()
{

	for (std::vector<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		(*it)->Draw();
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

	for (int i = 0; i < models.size(); ++i) {
		delete models[i];
	}

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

