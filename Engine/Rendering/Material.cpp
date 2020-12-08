#include "Material.h"
#include "../Modules/ModuleTextures.h"
#include "../Application.h"
Material::Material(std::string name, std::string path, unsigned id, std::pair<int, int> texSize) :id(id), texSize(texSize), textureName(name), texturePath(path) {

}

Material::~Material() {

}


Material::Material(aiMaterial* mat, aiString file, std::string& materialPath, std::string modelPath) : id(0) {

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

	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS) {
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
	if (!success) {
		LOG("Failed to load %s", matName);
	}
	else {

		id = tex;
		//materials.push_back(Material(matName, materialPath.c_str(), tex, texSize));
	}

}



Material::Material(aiMaterial* mat, std::string modelPath) : id(0) {

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
	aiString file = aiString(modelPath);
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS) {
		LOG("Trying to load  %s as Diffuse texture", matName);
		//materialPath = file.C_Str();
		success = App->textures->LoadTexture(file.C_Str(), tex, texSize);

		texturePath = file.C_Str();


		std::string withModelPath = modelPath.substr(0, lastSlash) + "\\" + file.C_Str();
		std::string withTexturesFolderPath = App->textures->GetTexturesFolderName() + "\\" + file.C_Str();
		if (!success) {
			success = App->textures->LoadTexture(withModelPath, tex, texSize);
			//materialPath = withModelPath;
			texturePath = withModelPath;
		}
		if (!success) {
			success = App->textures->LoadTexture(withTexturesFolderPath, tex, texSize);
			//materialPath = withTexturesFolderPath;
			texturePath = withTexturesFolderPath;
		}

	}
	if (!success) {
		LOG("Failed to load %s", matName);
	}
	else {
		textureName = matName;

		id = tex;
		//materials.push_back(Material(matName, materialPath.c_str(), tex, texSize));
	}

}

const unsigned Material::GetTextureID()const {
	//return 0;
	return id;
}

const std::pair<int, int> Material::GetTextureSize()const {
	return texSize;
}

const std::string Material::GetTextureName()const {
	return textureName;
}


const std::string Material::GetTexturePath()const {
	return texturePath;
}