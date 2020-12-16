#ifndef SKYBOX_H_
#define SKYBOX_H_
#include <glew.h>
#include <string>
#include "MathGeoLib/Math/float4x4.h"
class Shader;


class Skybox {
private:
	unsigned int skyboxVAO, skyboxVBO;
	std::string faces[6];
	Shader* skyboxShader;
	unsigned int texture;
public:
	Skybox(std::string[6], const char*, const char*);
	~Skybox();
	void CreateVAO();
	void Draw(float4x4 view, float4x4 proj);
};
#endif
