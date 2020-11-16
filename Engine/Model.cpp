#include "Model.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Utilities/Globals.h"
#include "Application.h"
#include "Modules/ModuleTextures.h"
void Model::Draw() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw(materials);
	}
}

std::string nameAsPNG(aiString name) {
	std::string stringName = name.C_Str();
	return stringName + ".png";
}

void Model::Load(const char* file_name)
{
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		for (int i = 0; i < scene->mNumMaterials; ++i) {
			materials.push_back(App->textures->LoadTexture(nameAsPNG(scene->mMaterials[i]->GetName()).c_str()));
		}
		meshes.reserve(scene->mNumMeshes);

		for (int i = 0; i < scene->mNumMeshes; ++i) {
			meshes.push_back(Mesh(scene->mMeshes[i]));
		}

		// TODO: LoadTextures(scene->mMaterials, scene->mNumMaterials);
		// TODO: LoadMeshes(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}


void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->textures->LoadTexture(file.data));
		}
	}
}