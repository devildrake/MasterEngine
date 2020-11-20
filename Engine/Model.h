#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>

class Model {

private:
	std::vector<unsigned> materials;
	std::vector<Mesh*> meshes;
	std::string file_name;
public:
	Model(const char* file_name);
	Model();
	~Model();
	const bool Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void Draw();
	const std::string GetFileName()const;
	const static bool SceneFound(const char* file_name);
};

#endif