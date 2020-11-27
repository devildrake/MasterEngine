#include "Model.h"
#include "Utilities/Globals.h"
#include "Application.h"
#include "Modules/ModuleTextures.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Leaks.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(const char* new_file) :transform(Transform()) {
	Load(new_file);
}

Model::~Model() {
	ReleaseTextures();
	materials.clear();

	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		delete* it;
	}

	meshes.clear();
}

const float3  Model::GetBoundingCenter()const {
	return float3(boundingBox.first.Lerp(boundingBox.second, 0.5f));
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

Model::Model() :transform(Transform()) {

}

void Model::ReleaseTextures() {

	for (std::vector<Material>::iterator it = materials.begin(); it != materials.end(); ++it) {
		App->textures->ReleaseTexture((*it).GetTexturePath());
	}

}

void Model::ResetTransform() {
	SetPos(float3::zero);
	SetRotation(float3::zero);
	SetScale(float3::one);
}

void Model::LoadMaterial(aiMaterial* mat, aiString file, std::string& materialPath, std::string modelPath) {

	//		if (mat.Get(AI_MATKEY_NAME, aiTextureType_DIFFUSE, 0, aName)) {


	const char* matName = mat->GetName().C_Str();
	GLuint tex;
	std::pair<int, int>texSize{ 0,0 };

	int lastSlash = 0;
	for (int i = modelPath.size(); i > 0 && lastSlash == 0; --i) {
		if (modelPath[i] == '\\') {
			lastSlash = i;
		}
	}

	bool success = false;


	if (mat->GetTexture(aiTextureType_NONE, 0, &file) == AI_SUCCESS) {
		LOG("%s was found to be a NONE type texture", matName);
	}
	else if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Diffuse texture", matName);
		materialPath = file.C_Str();
		success = App->textures->LoadTexture(file.C_Str(), tex, texSize);
		std::string withModelPath = modelPath.substr(0, lastSlash) + "\\" + file.C_Str();
		std::string withTexturesFolderPath = App->textures->GetTexturesFolderName() + "\\" + file.C_Str();
		if (!success) {
			success = App->textures->LoadTexture(withModelPath, tex, texSize);
			materialPath = withModelPath;

		}
		if (!success) {
			success = App->textures->LoadTexture(withTexturesFolderPath, tex, texSize);
			materialPath = withTexturesFolderPath;

		}

	}
	else if (mat->GetTexture(aiTextureType_SPECULAR, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Specular texture", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Ambient texture", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_EMISSIVE, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Emissive", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_HEIGHT, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as HeightMap", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_NORMALS, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Normals", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_SHININESS, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Shininess", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_OPACITY, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Opacity", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_DISPLACEMENT, 0, &file) == AI_SUCCESS) {
		LOG("Loaded %s as Displacement", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_LIGHTMAP, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as LightMap", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_REFLECTION, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Reflection ", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_BASE_COLOR, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Base Color", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Normal camera", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_EMISSION_COLOR, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Emission color", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_METALNESS, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Metalness ", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Diffuse roughness", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Ambient Oclussion", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	else if (mat->GetTexture(aiTextureType_UNKNOWN, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Unknown texture", matName);
		materialPath = file.C_Str();
		if (App->textures->LoadTexture(file.C_Str(), tex)) {
			success = true;
		}
	}
	//else {
	//	LOG("Trying to load a hardcoded .png with material name %s", matName);
	//	*materialPath = nameAsPNG(aiString(matName));

	//	std::string actualFilePath = file.C_Str() + *materialPath;

	//	if (App->textures->LoadTexture(*materialPath, &tex)) {
	//		success = true;
	//	}
	//}

	if (!success) {
		LOG("Failed to load %s", matName);
	}
	else {
		materials.push_back(Material(matName, materialPath.c_str(), tex, texSize));
	}

}

const bool Model::Load(const char* file_name)
{

	LOG("Trying to load %s...", file_name);

	//const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes);

	boundingBox.first = float3(10000, 10000, 10000);
	boundingBox.second = float3(0, 0, 0);

	if (scene)
	{
		this->file_name = file_name;

		aiString file;
		std::string materialPath;

		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {

			//PREVIOUSLY METHOD A 
			LoadMaterial(scene->mMaterials[i], file, materialPath, file_name);

		}
		meshes.reserve(scene->mNumMeshes);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			meshes.push_back(new Mesh(scene->mMeshes[i], materialPath.c_str()));

			if (scene->mMeshes[i]->mAABB.mMin.x < boundingBox.first.x) {
				boundingBox.first.x = scene->mMeshes[i]->mAABB.mMin.x;
			}
			if (scene->mMeshes[i]->mAABB.mMin.y < boundingBox.first.y) {
				boundingBox.first.y = scene->mMeshes[i]->mAABB.mMin.y;
			}
			if (scene->mMeshes[i]->mAABB.mMin.z < boundingBox.first.z) {
				boundingBox.first.z = scene->mMeshes[i]->mAABB.mMin.z;
			}



			if (scene->mMeshes[i]->mAABB.mMax.x > boundingBox.second.x) {
				boundingBox.second.x = scene->mMeshes[i]->mAABB.mMax.x;
			}
			if (scene->mMeshes[i]->mAABB.mMax.y > boundingBox.second.y) {
				boundingBox.second.y = scene->mMeshes[i]->mAABB.mMax.y;
			}
			if (scene->mMeshes[i]->mAABB.mMax.z > boundingBox.second.z) {
				boundingBox.second.z = scene->mMeshes[i]->mAABB.mMax.z;
			}

		}
		return true;
	}
	else
	{
		LOG("Error loading %lu: %lu", file_name, aiGetErrorString());
	}
	return false;
}

const std::pair<float3, float3> Model::BoundingBox()const {
	return std::pair<float3, float3>(float3(boundingBox.first.x * transform.scale.x, boundingBox.first.y * transform.scale.y, boundingBox.first.z * transform.scale.z), float3(boundingBox.second.x * transform.scale.x, boundingBox.second.y * transform.scale.y, boundingBox.second.z * transform.scale.z));
}