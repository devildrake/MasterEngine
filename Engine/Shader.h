#ifndef SHADER_H
#define SHADER_H

class Shader {
private:
	unsigned id;
public:

	char* LoadShaderSource(const char* shader_file_name);
	unsigned	CompileShader(unsigned type, const char* source);
	unsigned	CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	Shader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
	~Shader();
	unsigned GetID();
};

#endif