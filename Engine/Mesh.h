#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>

class aiMesh;

class Mesh {
private:
	unsigned vbo, ebo, vao;
	int num_vertices, num_indices, material_index;
	std::string texture_path;
public:

	Mesh(const aiMesh* mesh);
	Mesh(const aiMesh* mesh, const char* Matname);
	~Mesh();
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);
	void ReleaseTextures();
	void SetTexture(int index, std::string path);
};

#endif