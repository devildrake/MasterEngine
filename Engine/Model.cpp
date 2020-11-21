#include "Model.h"
#include "Utilities/Globals.h"
#include "Application.h"
#include "Modules/ModuleTextures.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Leaks.h"

Model::Model(const char* new_file) :transform(Transform()) {
	Load(new_file);
}

Model::~Model() {
	materials.clear();

	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		delete* it;
	}

	meshes.clear();
}

void Model::SetRotation(float3 newRot) {
	transform.rotation = newRot;
}

void Model::SetScale(float3 newScale) {
	transform.scale = newScale;
}

void Model::SetPos(float3 newPos) {
	transform.position = newPos;
}

const float3 Model::Position()const {
	return transform.position;
}

const float3 Model::Scale()const {
	return transform.scale;
}

const float3 Model::Rotation()const {
	return transform.rotation;
}

const int Model::GetVertices() const {
	int temp = 0;

	for (std::vector<Mesh*>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		temp += (*it)->GetVertices();
	}

	return temp;
}

const int Model::GetTris()const {
	int temp = 0;

	for (std::vector<Mesh*>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		temp += (*it)->GetTris();
	}

	return temp;
}

const bool Model::SceneFound(const char* file_name) {
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	return scene;
}

const std::string Model::GetFileName()const {
	return file_name;
}

void Model::Draw() {
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->Draw(materials, transform);
	}
}

std::string nameAsPNG(aiString name) {
	std::string stringName = name.C_Str();
	return stringName + ".png";
}

Model::Model() {

}

const bool Model::Load(const char* file_name)
{

	LOG("Trying to load %s...", file_name);

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		this->file_name = file_name;

		aiString file;
		std::string materialPath;

		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {

			LOG("Trying to load %s...", scene->mMaterials[i]->GetName().C_Str());

			//materialName = scene->mMaterials[i]->GetName();
			//materialPath = nameAsPNG(materialName);

			//materials.push_back(App->textures->LoadTexture(nameAsPNG(materialName).c_str()));


			//materials.push_back(App->textures->LoadTexture(scene->mMaterials[i]->GetName().C_Str()));
			aiMaterial* mat = scene->mMaterials[i];

			if (scene->mMaterials[i]->GetTexture(aiTextureType_NONE, 0, &file) == AI_SUCCESS) {
				LOG("%s was found to be a NONE type texture", scene->mMaterials[i]->GetName().C_Str());
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Diffuse texture", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Specular texture", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Ambient texture", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_EMISSIVE, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Emissive", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as HeightMap", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Normals", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_SHININESS, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Shininess", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_OPACITY, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Opacity", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_DISPLACEMENT, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Displacement", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_LIGHTMAP, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as LightMap", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_REFLECTION, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Reflection ", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_BASE_COLOR, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Base Color", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Normal camera", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_EMISSION_COLOR, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Emission color", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_METALNESS, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Metalness ", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Diffuse roughness", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Ambient Oclussion", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else if (scene->mMaterials[i]->GetTexture(aiTextureType_UNKNOWN, 0, &file) == AI_SUCCESS) {
				LOG("Loaded %s as Unknown texture", scene->mMaterials[i]->GetName().C_Str());
				materialPath = file.C_Str();
				materials.push_back(App->textures->LoadTexture(file.C_Str()));
			}
			else {
				LOG("Loaded a hardcoded .png with material name %s", scene->mMaterials[i]->GetName().C_Str());
				aiString materialName = scene->mMaterials[i]->GetName();
				materialPath = nameAsPNG(materialName);
				materials.push_back(App->textures->LoadTexture(nameAsPNG(materialName).c_str()));
			}

			//else if (scene->mMaterials[i]->GetTexture(aiTextureType_UNKNOWN, 0, &file) == AI_SUCCESS) {
			//	materials.push_back(App->textures->LoadTexture(file.C_Str()));
			//}
			//else {
			//	materials.push_back(App->textures->LoadTexture(file.C_Str()));
			//}

		}
		meshes.reserve(scene->mNumMeshes);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			meshes.push_back(new Mesh(scene->mMeshes[i], materialPath.c_str()));

		}
		return true;
		// TODO: LoadTextures(scene->mMaterials, scene->mNumMaterials);
		// TODO: LoadMeshes(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		LOG("Error loading %lu: %lu", file_name, aiGetErrorString());
	}
	return false;
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