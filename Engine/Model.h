#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
class Model {

private:
	std::vector<unsigned> materials;
	std::vector<Mesh> meshes;
public:
	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void Draw();
};

#endif