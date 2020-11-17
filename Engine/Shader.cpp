#include "Shader.h"
#include "glew.h"
#include "Utilities/Globals.h"
#include "Leaks.h"

Shader::Shader(const char* vertex_shader_file_name, const char* fragment_shader_file_name) {
	char* vsSource = LoadShaderSource(vertex_shader_file_name);
	char* fsSource = LoadShaderSource(fragment_shader_file_name);
	unsigned  vertexShader = CompileShader(GL_VERTEX_SHADER, vsSource);
	unsigned  fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsSource);
	id = CreateProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	delete vsSource;
	delete fsSource;
}


Shader::~Shader() {
	glUseProgram(0);
	glDeleteProgram(id);
}

unsigned Shader::GetID() {
	return id;
}

char* Shader::LoadShaderSource(const char* shader_file_name)
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


unsigned Shader::CompileShader(unsigned type, const char* source) {
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

unsigned Shader::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
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