#include "ModuleHelloTriangleExcerise.h"
#include "SDL.h"
#include <string>

ModuleHelloTriangleExcercise::ModuleHelloTriangleExcercise() {

}

ModuleHelloTriangleExcercise::~ModuleHelloTriangleExcercise() {

}

bool ModuleHelloTriangleExcercise::Init() {

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	char* fragmentShaderSource = LoadShaderSource("frag.fs");
	char* vertexShaderSource = LoadShaderSource("vert.vs");
	//std::string fragmentShaderSourceString(fragmentShaderSource);
	//std::string vertexShaderSourceString(vertexShaderSource);


	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	shaderID = CreateProgram(vertexShader, fragmentShader);

	vbo = CreateTriangleVBO();
	return true;
}

update_status ModuleHelloTriangleExcercise::PreUpdate() {
	return UPDATE_CONTINUE;

}
update_status ModuleHelloTriangleExcercise::Update() {
	RenderVBO(vbo);

	return UPDATE_CONTINUE;
}
update_status ModuleHelloTriangleExcercise::PostUpdate() {
	return UPDATE_CONTINUE;

}
bool ModuleHelloTriangleExcercise::CleanUp() {
	DestroyVBO(vbo);
	glDeleteProgram(shaderID);
	return true;
}

unsigned ModuleHelloTriangleExcercise::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	return vbo;
}

// This function must be called one time at creation of vertex buffer
void ModuleHelloTriangleExcercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

// This function must be called each frame for drawing the triangles
void ModuleHelloTriangleExcercise::RenderVBO(unsigned vbo)
{
	glUseProgram(shaderID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

char* ModuleHelloTriangleExcercise::LoadShaderSource(const char* shader_file_name)
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


unsigned ModuleHelloTriangleExcercise::CompileShader(unsigned type, const char* source) {
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
			//#if DEBUG
			LOG("Log Info: %s", info);
			//#endif

			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleHelloTriangleExcercise::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
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
#if DEBUG
			LOG("Program Log Info: %s", info);
#endif

			free(info);
		}
	}

	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

void ModuleHelloTriangleExcercise::RenderVBO(unsigned vbo, unsigned program)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}