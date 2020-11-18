#include "Model.h"
#include "Utilities/Globals.h"
#include "Application.h"
#include "Modules/ModuleTextures.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Leaks.h"

Model::Model(const char* file_name) {
	Load(file_name);
}

Model::~Model() {
	materials.clear();

	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		delete* it;
	}

	meshes.clear();
}

void Model::Draw() {
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->Draw(materials);
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
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
			materials.push_back(App->textures->LoadTexture(nameAsPNG(scene->mMaterials[i]->GetName()).c_str()));
		}
		meshes.reserve(scene->mNumMeshes);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			meshes.push_back(new Mesh(scene->mMeshes[i]));
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