#ifndef  MODULE_TRANSFORMED_HELLO_TRIANGLE_EXCERCISE
#define MODULE_TRANSFORMED_HELLO_TRIANGLE_EXCERCISE

#include "../Modules/Module.h"
#include "../MathGeoLib/MathGeoLib.h"
#include <glew.h>
#include "../MathGeoLib/Geometry/Frustum.h"
#include <Globals.h>
class ModuleTransformedTriangleExcercise :public Module {
public:
	ModuleTransformedTriangleExcercise();
	~ModuleTransformedTriangleExcercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);
	unsigned vbo;
	unsigned fragmentShader, vertexShader;
	unsigned shaderID;
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	float3 trianglePos;
};

#endif