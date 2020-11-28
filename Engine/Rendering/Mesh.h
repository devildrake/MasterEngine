#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "../MathGeoLib/MathGeoLib.h"

class aiMesh;
class Transform;
class Material;

class Mesh {
private:
	unsigned vbo, ebo, vao;
	int num_vertices, num_indices, material_index, num_faces;
	std::string texture_path;
public:

	Mesh(const aiMesh* mesh);
	Mesh(const aiMesh* mesh, const char* Matname);
	~Mesh();
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<Material>& model_textures, Transform transform);
	void SetTexture(int index, std::string path);
	const int GetTris()const;
	const int GetVertices()const;
};

#endif