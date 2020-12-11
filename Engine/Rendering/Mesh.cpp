#include "Mesh.h"
#include <glew.h>
#include <assimp/mesh.h>

#include "../Application.h"
#include "../Modules/ModuleRender.h"
#include "../Modules/ModuleEditorCamera.h"
//#include "../Modules/ModuleTextures.h"
//#include "../Components/ComponentTransform.h"
#include "Material.h"
//#include <Leaks.h>

Mesh::Mesh(const aiMesh* mesh) {
	LoadAABB(mesh);
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
}

Mesh::Mesh(const aiMesh* mesh, const char* matname) {
	texture_path = std::string(matname);
	LoadAABB(mesh);
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
}


Mesh::~Mesh() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}



void Mesh::SetTexture(int index, std::string path) {
	texture_path = path;
	material_index = index;
}

const int& Mesh::GetTris()const {
	return num_faces;
}

const int& Mesh::GetVertices()const {
	return num_vertices;
}

const AABB& Mesh::GetAABB()const {
	return aabb;
}

void Mesh::LoadAABB(const aiMesh* mesh) {
	aabb.minPoint.x = mesh->mAABB.mMin.x;
	aabb.minPoint.y = mesh->mAABB.mMin.y;
	aabb.minPoint.z = mesh->mAABB.mMin.z;

	aabb.maxPoint.x = mesh->mAABB.mMax.x;
	aabb.maxPoint.y = mesh->mAABB.mMax.y;
	aabb.maxPoint.z = mesh->mAABB.mMax.z;
}


void Mesh::LoadVBO(const aiMesh* mesh) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	//float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	num_vertices = mesh->mNumVertices;
}

void Mesh::LoadEBO(const aiMesh* mesh) {
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	//unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_MAP_WRITE_BIT));
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_indices = mesh->mNumFaces * 3;
	num_faces = mesh->mNumFaces;
}

void Mesh::CreateVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
	glBindVertexArray(0);
}

//void Mesh::Draw(const std::vector<Material>& model_textures, float4x4 transformationMatrix) {
//	unsigned program = App->renderer->GetDefaultShaderID();
//	const float4x4& view = App->editorCamera->GetFrustum()->ViewMatrix();
//	const float4x4& proj = App->editorCamera->GetFrustum()->ProjectionMatrix();
//	//float4x4 model = float4x4::identity;
//	//model = float4x4::RotateX(DegToRad(transform.rotation.x)) * float4x4::RotateY(DegToRad(transform.rotation.y)) * float4x4::RotateZ(DegToRad(transform.rotation.z)) * float4x4::Translate(transform.position) * float4x4::Scale(transform.localScale) * model;
//	//model = float4x4::Translate(transform.CalculateGlobalPosition()) * float4x4::RotateX(DegToRad(transform.rotation.x)) * float4x4::RotateY(DegToRad(transform.rotation.y)) * float4x4::RotateZ(DegToRad(transform.rotation.z)) * float4x4::Scale(transform.localScale) * model;
//
//	glUseProgram(program);
//	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&transformationMatrix);
//	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
//	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
//	glActiveTexture(GL_TEXTURE0);
//	//material_index = 0;
//	//glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
//	//for (std::vector<Material>::const_iterator it = model_textures.begin(); it != model_textures.end(); ++it) {
//	//	glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
//	//	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
//	//	material_index++;
//	//}
//
//	material_index = 0;
//	glBindTexture(GL_TEXTURE_2D, (model_textures)[material_index].GetTextureID());
//
//
//	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
//
//	//glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
//	//glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
//	glBindVertexArray(0);
//}

void Mesh::Draw(const Material& mat, float4x4 transformationMatrix) {
	unsigned program = App->renderer->GetDefaultShaderID();
	const float4x4& view = App->editorCamera->GetFrustum()->ViewMatrix();
	const float4x4& proj = App->editorCamera->GetFrustum()->ProjectionMatrix();
	//float4x4 model = float4x4::identity;
	//model = float4x4::RotateX(DegToRad(transform.rotation.x)) * float4x4::RotateY(DegToRad(transform.rotation.y)) * float4x4::RotateZ(DegToRad(transform.rotation.z)) * float4x4::Translate(transform.position) * float4x4::Scale(transform.localScale) * model;
	//model = float4x4::Translate(transform.CalculateGlobalPosition()) * float4x4::RotateX(DegToRad(transform.rotation.x)) * float4x4::RotateY(DegToRad(transform.rotation.y)) * float4x4::RotateZ(DegToRad(transform.rotation.z)) * float4x4::Scale(transform.localScale) * model;

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&transformationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	//material_index = 0;
	//glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	//for (std::vector<Material>::const_iterator it = model_textures.begin(); it != model_textures.end(); ++it) {
	//	glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	//	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	//	material_index++;
	//}

	//material_index = 0;
	glBindTexture(GL_TEXTURE_2D, mat.GetTextureID());


	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);

	//glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	//glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}