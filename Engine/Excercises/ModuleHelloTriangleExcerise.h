#ifndef  MODULE_HELLO_TRIANGLE_EXCERCISE
#define MODULE_HELLO_TRIANGLE_EXCERCISE

#include "../Modules/Module.h"
#include <glew.h>

class ModuleHelloTriangleExcercise :public Module {
public:
	ModuleHelloTriangleExcercise();
	~ModuleHelloTriangleExcercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();



private:


	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);
	void RenderVBO(unsigned vbo, unsigned program);
	unsigned vbo;
	unsigned fragmentShader, vertexShader;
	unsigned shaderID;
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

};



#endif // ! MODULE_HELLO_TRIANGLE_EXCERCISE