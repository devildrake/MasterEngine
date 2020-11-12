#ifndef MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_H
#define MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_Hs
#include "../Modules/Module.h"
#include "../MathGeoLib/MathGeoLib.h"
#include "glew.h"
#include "../MathGeoLib/Geometry/Frustum.h"
#include "../Utilities/Globals.h"
class ModuleTransformedTexturedTriangleExcercise :public Module {
public:
	ModuleTransformedTexturedTriangleExcercise();
	~ModuleTransformedTexturedTriangleExcercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	unsigned CreeateQuadVBO();
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