#include "ModuleTransformedTexturedTriangleExcercise.h"

#include "ModuleTransformedTriangleExcercise.h"
#include "SDL.h"
#include <string>
#include "../Modules/ModuleEditorCamera.h"
#include "../Application.h"
#include "../Utilities/debug_draw.hpp"
#include "../Modules/ModuleTextures.h"
ModuleTransformedTexturedTriangleExcercise::ModuleTransformedTexturedTriangleExcercise() {
	info = new ILinfo();


	ui = ExcerciseUI();
}

ModuleTransformedTexturedTriangleExcercise::~ModuleTransformedTexturedTriangleExcercise() {

}

#if _DEBUG
void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

bool ModuleTransformedTexturedTriangleExcercise::Init() {
#if _DEBUG
	LOG("DEBUG enabled");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
		true);
#endif
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	char* fragmentShaderSource = LoadShaderSource("transformedTexturedTriangleExFrag.fs");
	char* vertexShaderSource = LoadShaderSource("transformedTexturedTriangleExVert.vs");
	//std::string fragmentShaderSourceString(fragmentShaderSource);
	//std::string vertexShaderSourceString(vertexShaderSource);


	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	shaderID = CreateProgram(vertexShader, fragmentShader);

	vbo = CreeateQuadVBO();//CreateTriangleVBO();

	float3 pos = App->editorCamera->GetFrustum()->Pos();
	App->editorCamera->GetFrustum()->SetFront(math::vec(0, 0, -1.0f));

	trianglePos = pos - float3(0, 0, 10);

	return true;
}

bool ModuleTransformedTexturedTriangleExcercise::Start() {
	texture = App->textures->LoadTexture("Lenna.png");

	return true;
}

update_status ModuleTransformedTexturedTriangleExcercise::PreUpdate() {
	return UPDATE_CONTINUE;

}
update_status ModuleTransformedTexturedTriangleExcercise::Update() {

	RenderVBO(vbo);
	iluGetImageInfo(info);
	ui.Draw(info);

	return UPDATE_CONTINUE;
}
update_status ModuleTransformedTexturedTriangleExcercise::PostUpdate() {
	return UPDATE_CONTINUE;

}
bool ModuleTransformedTexturedTriangleExcercise::CleanUp() {
	DestroyVBO(vbo);
	glDeleteProgram(shaderID);
	return true;
}

unsigned ModuleTransformedTexturedTriangleExcercise::CreeateQuadVBO() {
	float vtx_data[] = {
			0.5f,  0.5f, 0.0f,	1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,	1.0f, 0.0f, // bottom right
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,  // top left 
			0.5f, -0.5f, 0.0f,	1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f  // top left 
	};

	/*
	float vtx_data[] = {
			0.5f,  0.5f, 0.0f,	1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,	1.0f, 0.0f, // bottom right
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,  // top left
			0.5f, -0.5f, 0.0f,	1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f  // top left
	};*/
	unsigned newVbo;
	glGenBuffers(1, &newVbo);
	glBindBuffer(GL_ARRAY_BUFFER, newVbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return newVbo;
}
unsigned ModuleTransformedTexturedTriangleExcercise::CreateTriangleVBO()
{
	//float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

//	float vtx_data[] = {
//-1.0f, -1.0f, 0.0f, // ← v0 pos
//1.0f, -1.0f, 0.0f, // ← v1 pos
//0.0f, 1.0f, 0.0f, // ← v2 pos
//
//0.0f, 0.0f, // ← v0 texcoord
//1.0f, 0.0f, // ← v1 texcoord
//0.5f, 1.0f // ← v2 texcoord
//};
	float vtx_data[] = {
		-1.0f, -1.0f, 0.0f	,		0.0f, 0.0f,
		1.0f, -1.0f, 0.0f	,		1.0f, 0.0f,
		0.0f, 1.0f, 0.0f	, 		0.5f, 1.0f
	};

	//Manually Flipped
	//	float vtx_data[] = {
	//	-1.0f, -1.0f, 0.0f	,		0.0f, 1.0f,
	//	1.0f, -1.0f, 0.0f	,		1.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f	, 		0.5f, 0.0f
	//};


	//float vtx_data[] = {
	//-1.0f, 1.0f,   0.0f,	0, 0,
	//1.0f, 1.0f,    0.0f,	1, 0,
	//- 1.0f, -1.0f, 0.0f, 0, 1.0f };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return vbo;
}

// This function must be called one time at creation of vertex buffer
void ModuleTransformedTexturedTriangleExcercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

//// This function must be called each frame for drawing the triangles
//void ModuleTransformedTriangleExcercise::RenderVBO(unsigned vbo)
//{
//	glUseProgram(shaderID);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glEnableVertexAttribArray(0);
//	// size = 3 float per vertex
//	// stride = 0 is equivalent to stride = sizeof(float)*3
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	// 1 triangle to draw = 3 vertices
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//}

 //This function must be called each frame for drawing the triangles
void ModuleTransformedTexturedTriangleExcercise::RenderVBO(unsigned vbo)
{
	glUseProgram(shaderID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	float4x4 model, view, projection;

	projection = App->editorCamera->GetFrustum()->ProjectionMatrix();
	view = App->editorCamera->GetFrustum()->ViewMatrix();

	model = float4x4::identity;
	model.SetTranslatePart(trianglePos);
	//float4x4 translateMat = float4x4::Translate(trianglePos);
	//model = translateMat * model;

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_TRUE, &projection[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderID, "mytexture"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	//sglDrawArrays(GL_TRIANGLES, 3, 3);
}

char* ModuleTransformedTexturedTriangleExcercise::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);

		fseek(file, 0, SEEK_SET);

		data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}


unsigned ModuleTransformedTexturedTriangleExcercise::CompileShader(unsigned type, const char* source) {
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
#if _DEBUG
			LOG("Log Info: %s", info);
#endif

			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleTransformedTexturedTriangleExcercise::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;

			char* info = (char*)malloc(len);

			glGetProgramInfoLog(program_id, len, &written, info);
#if _DEBUG
			LOG("Program Log Info: %s", info);
#endif

			free(info);
		}
	}

	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}
