#include <Globals.h>
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include <glew.h>
#include "../Rendering/Model.h"
#include "../Rendering/Shader.h"
#include <assimp/cimport.h>
#include <Leaks.h>

ModuleRender::ModuleRender() :bgColor(0.2f, 0.2f, 0.2f), context(nullptr), default_shader(nullptr), glcontext(nullptr), faceCulling(true), depthTest(true), wireFramePolygonMode(false), quadShader(nullptr) {

}

// Destructor
ModuleRender::~ModuleRender()
{
	if (default_shader != nullptr)
		delete default_shader;
}

void myCallback(const char* msg, char* userData) {
	LOG(msg);
}

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
// positions   // texCoords
-1.0f,  1.0f,  0.0f, 1.0f,
-1.0f, -1.0f,  0.0f, 0.0f,
 1.0f, -1.0f,  1.0f, 0.0f,

-1.0f,  1.0f,  0.0f, 1.0f,
 1.0f, -1.0f,  1.0f, 0.0f,
 1.0f,  1.0f,  1.0f, 1.0f
};

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits

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

		//Framebuffers
	quadShader = new Shader("renderQuad.vs", "renderQuad.fs");
	glGenFramebuffers(1, &framebuffer);

	RegenerateRenderBuffer();

	//Quad VAO 

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	return true;
}

void ModuleRender::RegenerateRenderBuffer() {

	if (rbo != 0)glDeleteRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(1, &rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->GetWidth(), App->window->GetHeight());

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// generate texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->GetWidth(), App->window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error generating framebuffer");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void ModuleRender::ToggleWireFrameMode()const {
	if (wireFramePolygonMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ModuleRender::ToggleDepthTest()const {
	if (depthTest) {
		glEnable(GL_DEPTH_TEST); // Enable cull backward faces
	}
	else {
		glDisable(GL_DEPTH_TEST); // Enable cull backward faces
	}
}


void ModuleRender::ToggleFaceCulling()const {
	if (faceCulling) {
		glEnable(GL_CULL_FACE); // Enable cull backward faces
	}
	else {
		glDisable(GL_CULL_FACE); // Enable cull backward faces
	}

}


void ModuleRender::AddModel(Model* m) {
	if (m == nullptr)return;
	models.push_back(m);
}

void ModuleRender::RemoveModel(Model* m) {
	if (m == nullptr)return;
	models.remove(m);
}

bool ModuleRender::Start() {
	default_shader = new Shader("texturedModelVert.vs", "texturedModelFrag.fs");
	App->input->SetLastFileDroppedOnWindow("BakerHouse.fbx");
	return true;
}

update_status ModuleRender::PreUpdate()
{
	SDL_GetWindowSize(App->window->window,
		&App->window->screen_surface->w, &App->window->screen_surface->h);

	glViewport(0, 0, App->window->screen_surface->w, App->window->screen_surface->h);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	//GenerateRenderBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.


	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

const unsigned ModuleRender::GetDefaultShaderID() const {
	return default_shader->GetID();
}



// Called every draw update
update_status ModuleRender::Update()
{

	for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		(*it)->Draw();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		 // clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(quadShader->GetID());
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);


	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	models.clear();

	if (default_shader != nullptr)
	{
		delete default_shader;
		default_shader = nullptr;
	}

	if (quadShader != nullptr) {
		delete quadShader;
		quadShader = nullptr;
	}

	glDeleteRenderbuffers(1, &rbo);
	glDeleteRenderbuffers(1, &framebuffer);

	aiDetachAllLogStreams();
	//Destroy window
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	RegenerateRenderBuffer();
}

SDL_GLContext ModuleRender::GetContext() {
	return glcontext;
}

