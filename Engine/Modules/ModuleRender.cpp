#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include <glew.h>
#include "../Model.h"
#include "../Shader.h"
#include "../Leaks.h"
#include <assimp/cimport.h>

ModuleRender::ModuleRender() {
	bgColor = float3(0.2f, 0.2f, 0.2f);
}

// Destructor
ModuleRender::~ModuleRender()
{
	if (default_shader != nullptr)
		delete default_shader;
}

void myCallback(const char* msg, char* userData) {
	LOG(msg);
	//printIntoMyFile(msg);
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




	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	return true;
}

bool ModuleRender::Start() {
	default_shader = new Shader("texturedModelVert.vs", "texturedModelFrag.fs");
	App->input->SetLastFileDroppedOnWindow("BakerHouse.fbx");
	//currentModel = new Model("PenguinBaseMesh.obj");
	currentModel = new Model("BakerHouse.fbx");
	models.push_back(currentModel);
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
	const char* lastFile = App->input->GetLastFileDroppedOnWindow();

	if (lastFile != nullptr) {

		if (currentModel->GetFileName() != lastFile) {

			Model* newModel = new Model();
			if (Model::SceneFound(lastFile)) {
				models.remove(currentModel);
				delete currentModel;
				if (newModel->Load(lastFile)) {
					currentModel = newModel;
					models.push_back(currentModel);
				}
				else {
					currentModel = nullptr;
					delete newModel;
				}
			}
			else {
				delete newModel;
			}
		}
	}

	for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		(*it)->Draw();
	}

	return UPDATE_CONTINUE;
}

//
//bool ModuleRender::AddModel(Model* model) {
//	modelsToDraw.push_back(model);
//	return true;
//}
//
//bool ModuleRender::RemoveModel(Model* model) {
//	if (model == nullptr) return true;
//
//	std::vector<Model*>::iterator it = modelsToDraw.begin();
//	bool found = false;
//	while (!found && it != modelsToDraw.end()) {
//		if ((*it) == model) {
//			modelsToDraw.erase(it);
//			found = true;
//		}
//		++it;
//	}
//
//	return found;
//}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		delete* it;
	}
	models.clear();

	if (default_shader != nullptr)
	{
		delete default_shader;
		default_shader = nullptr;
	}

	aiDetachAllLogStreams();
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

