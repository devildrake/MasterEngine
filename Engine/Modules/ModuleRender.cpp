#include <Globals.h>
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include <glew.h>
//#include "../Rendering/Model.h"
#include "../Rendering/Shader.h"
#include <assimp/cimport.h>
#include <Leaks.h>
#include <Brofiler.h>
ModuleRender::ModuleRender() :Module("Renderer"), bgColor(0.2f, 0.2f, 0.2f), context(nullptr), defaultShader(nullptr), glcontext(nullptr), faceCulling(true), depthTest(true), wireFramePolygonMode(false), quadShader(nullptr) {

}

// Destructor
ModuleRender::~ModuleRender() {
	if (defaultShader != nullptr)
		delete defaultShader;
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


float cubeVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
};

// Called before render is available
bool ModuleRender::Init() {
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

	unlitShader = new Shader("Resources\\Shaders\\unlit.vs", "Resources\\Shaders\\unlit.fs");

	//Cube VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
	glGenTextures(1, &texColorBuffer);






	return true;
}

unsigned ModuleRender::GetRenderTextureID()const {
	return texColorBuffer;
}

//void ModuleRender::SceneWindowResized(unsigned int newW, unsigned int newH) {
//	RegenerateRenderBuffer();
//}

#include "../MathGeoLib/Math/float2.h"

void ModuleRender::RegenerateRenderBuffer() {

	if (rbo != 0)glDeleteRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(1, &rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	//This may need to be the size of the Imguiwindow?
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->GetWidth(), App->window->GetHeight());

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// generate texture
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	float2 size = float2(App->window->GetWidth(), App->window->GetHeight());

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
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ModuleRender::ToggleDepthTest()const {
	if (depthTest) {
		glEnable(GL_DEPTH_TEST); // Enable cull backward faces
	} else {
		glDisable(GL_DEPTH_TEST); // Enable cull backward faces
	}
}


void ModuleRender::ToggleFaceCulling()const {
	if (faceCulling) {
		glEnable(GL_CULL_FACE); // Enable cull backward faces
	} else {
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
	RegenerateRenderBuffer();

	defaultShader = new Shader("texturedModelVert.vs", "texturedModelFrag.fs");
	App->input->SetLastFileDroppedOnWindow("BakerHouse.fbx");
	return true;
}

UpdateStatus ModuleRender::PreUpdate() {

	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	//GenerateRenderBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	//In case there's no skybox, which manages clearing, we need to manually clear buffers
	if (!App->scene->DrawSkyBox()) {
		//glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	}

	return UPDATE_CONTINUE;
}

const unsigned& ModuleRender::GetDefaultShaderID() const {
	return defaultShader->GetID();
}

const unsigned& ModuleRender::GetUnlitShaderID() const {
	return unlitShader->GetID();
}

const unsigned& ModuleRender::GetCubeVAO()const {
	return cubeVAO;
}

// Called every draw update
UpdateStatus ModuleRender::Update() {

	//for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
	//	(*it)->Draw();
	//}

	return UPDATE_CONTINUE;
}

UpdateStatus ModuleRender::PostUpdate() {
	//Clearing and binding buffer 0 happens in editor (Probably shouldn't)

	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	//		 // clear all relevant buffers
	//	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	//	glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(quadShader->GetID());
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);

	BROFILER_CATEGORY("Renderer Post Update", Profiler::Color::Orchid);
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp() {
	LOG("Destroying renderer");

	for (std::list<Model*>::iterator it = models.begin(); it != models.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	models.clear();

	if (defaultShader != nullptr) {
		RELEASE(defaultShader);
	}

	if (quadShader != nullptr) {
		RELEASE(quadShader);
	}

	if (unlitShader != nullptr) {
		RELEASE(unlitShader);
	}

	glDeleteRenderbuffers(1, &rbo);
	glDeleteRenderbuffers(1, &framebuffer);

	aiDetachAllLogStreams();
	//Destroy window
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRender::MainWindowResized(unsigned width, unsigned height) {

	SDL_GetWindowSize(App->window->window,
		&App->window->screen_surface->w, &App->window->screen_surface->h);

	glViewport(0, 0, App->window->screen_surface->w, App->window->screen_surface->h);
	RegenerateRenderBuffer();
}

SDL_GLContext ModuleRender::GetContext() {
	return glcontext;
}

