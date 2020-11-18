#ifndef MESH_H
#define MESH_H

#include <vector>

class aiMesh;

class Mesh {
private:
	unsigned vbo, ebo, vao;
	int num_vertices, num_indices, material_index;

public:
	Mesh(const aiMesh* mesh);
	~Mesh();
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
};

#endif