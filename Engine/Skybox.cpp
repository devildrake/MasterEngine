#include "Skybox.h"
#include "Rendering/Shader.h"
#include "Modules/ModuleTextures.h"
#include "Application.h"
#include "Leaks.h"

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

void Skybox::CreateVAO() {

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

Skybox::Skybox(std::string aFaces[6], const char* shaderVs, const char* shaderFs) {

	skyboxShader = new Shader(shaderVs, shaderFs);

	for (int i = 0; i < 6; ++i) {
		faces[i] = aFaces[i];
	}
	CreateVAO();
	texture = App->textures->LoadCubeMap(aFaces);
}

Skybox::~Skybox() {
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	App->textures->ReleaseCubeMap(texture);
	delete skyboxShader;
}

void Skybox::Draw(float4x4 view, float4x4 proj) {

	glDepthFunc(GL_ALWAYS);

	glUseProgram(skyboxShader->GetID());

	//Set view and projection matrix, passed as parameters so that we may change the camera

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetID(), "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetID(), "projection"), 1, GL_TRUE, &proj[0][0]);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}