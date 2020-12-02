#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
#include "../Components/Transform.h"
#include "Material.h"


class Model {

private:
	Transform transform;
	std::vector<Material> materials;
	std::vector<Mesh*> meshes;
	std::string file_name;
	std::pair<float3, float3 >boundingBox;
public:
	Model(const char* file_name);
	Model();
	~Model();
	const bool Load(const char* file_name);
	//void LoadMaterials(const aiScene* scene);
	void LoadMaterial(const char* matName, std::string materialPath, unsigned tex, std::pair<int, int> texSize);
	void LoadMaterial(aiMaterial* mat, aiString file, std::string& materialPath, std::string modelPath);
	std::vector<Material>& GetMaterials();
	void Draw();
	const std::string GetFileName()const;
	const static bool SceneFound(const char* file_name);
	void SetPos(float3 newPos);
	void SetScale(float3 newScale);
	void SetRotation(float3 newRot);
	const float3 Position()const;
	const float3 Scale()const;
	const float3 Rotation()const;
	const float3 GetBoundingCenter()const;

	const int GetTris()const;
	const int GetVertices()const;
	const std::pair<float3, float3> BoundingBox()const;
	void ReleaseTextures();
	void ResetTransform();

public:
	friend class PropertiesWindow;
};

#endif