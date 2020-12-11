#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "../MathGeoLib/MathGeoLib.h"
#include <assimp/mesh.h>

class ComponentTransform;
class Material;

class Mesh {
private:
	unsigned vbo, ebo, vao;
	int num_vertices, num_indices, material_index, num_faces;
	std::string texture_path;
	AABB aabb;
public:

	Mesh(const aiMesh* mesh);
	Mesh(const aiMesh* mesh, const char* Matname);
	~Mesh();
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void LoadAABB(const aiMesh* mesh);
	void CreateVAO();
	//void Draw(const std::vector<Material>& model_textures, float4x4 transformationMat);
	void Draw(const Material& model_textures, float4x4 transformationMat);
	void SetTexture(int index, std::string path);
	const int& GetTris()const;
	const int& GetVertices()const;
	const AABB& GetAABB()const;
};

#endif