#include "ModuleTextures.h"
#include "IL/ilu.h"
#include "../Leaks.h"

ModuleTextures::ModuleTextures() : wrapMode(GL_REPEAT), minFilter(GL_LINEAR), magFilter(GL_LINEAR), texturesFolderName("Resources\\Textures") {}
ModuleTextures::~ModuleTextures() {

}

void ModuleTextures::SetMinMode(GLenum anEnum) {
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, anEnum);
	minFilter = anEnum;
}

void ModuleTextures::SetMagMode(GLenum anEnum) {
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, anEnum);
	magFilter = anEnum;
}

void ModuleTextures::SetWrapMode(GLenum anEnum) {
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, anEnum);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, anEnum);
	wrapMode = anEnum;
}

void ModuleTextures::ReleaseTexture(std::string path) {
	if (textureMap.size() > 0) {
		if (textureMap.find(path) != textureMap.end()) {
			glDeleteTextures(1, &textureMap[path]);
			textureMap.erase(path);
		}
	}
}

GLenum ModuleTextures::GetWrapMode() {
	return wrapMode;
}
GLenum ModuleTextures::GetMinFilter() {
	return minFilter;
}
GLenum ModuleTextures::GetMagFilter() {
	return magFilter;
}


const bool ModuleTextures::GenTexture(std::string path, GLuint& newTextureID) {
	ILboolean success;

	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
	if (!success)
	{
		// Error occured 
		return false;
	}

	glGenTextures(1, &newTextureID);
	glBindTexture(GL_TEXTURE_2D, newTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	ILinfo info = ILinfo();
	iluGetImageInfo(&info);

	//if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
	//	iluFlipImage();
	//}

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */

	glGenerateMipmap(GL_TEXTURE_2D);

	textureMap[path] = newTextureID;
	return success;
}

std::string ModuleTextures::GetTexturesFolderName() {
	return texturesFolderName;
}



const bool ModuleTextures::LoadTexture(std::string name, GLuint& tex, std::pair<int, int>& texSize) {
	ILboolean success;

	if (textureMap[name] != NULL) {
		return textureMap[name];
	}
	else {
		textureMap.erase(name);
	}


	ILuint newImageID;
	GLuint newTextureID = false;
	ilGenImages(1, &newImageID); /* Generation of one image name */
	ilBindImage(newImageID); /* Binding of image name */


	LOG("-----Trying to load texture with model specified path----- (%s)", name.c_str());
	success = ilLoadImage(name.c_str()); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{

		texSize.first = ilGetInteger(IL_IMAGE_WIDTH);
		texSize.second = ilGetInteger(IL_IMAGE_HEIGHT);

		success = GenTexture(name, newTextureID);
	}


	ilDeleteImages(1, &newImageID); /* Because we have already copied image data into texture data
	  we can release memory used by image. */

	tex = newTextureID;
	return success;
}


const bool ModuleTextures::LoadTexture(std::string name, GLuint& tex) {
	ILboolean success;

	if (textureMap[name] != NULL) {
		return textureMap[name];
	}
	else {
		textureMap.erase(name);
	}


	ILuint newImageID;
	GLuint newTextureID = false;
	ilGenImages(1, &newImageID); /* Generation of one image name */
	ilBindImage(newImageID); /* Binding of image name */


	LOG("-----Trying to load texture with model specified path----- (%s)", name.c_str());
	success = ilLoadImage(name.c_str()); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = GenTexture(name, newTextureID);
	}


	ilDeleteImages(1, &newImageID); /* Because we have already copied image data into texture data
	  we can release memory used by image. */

	tex = newTextureID;
	return success;
}


bool ModuleTextures::Init() {

	ilInit(); /* Initialization of DevIL */

	return true;
}

update_status ModuleTextures::PreUpdate() {
	return update_status::UPDATE_CONTINUE;
}


update_status ModuleTextures::Update() {

	return update_status::UPDATE_CONTINUE;
}
update_status ModuleTextures::PostUpdate() {
	return update_status::UPDATE_CONTINUE;
}

bool  ModuleTextures::CleanUp() {

	for (std::map < std::string, GLuint>::iterator it = textureMap.begin(); it != textureMap.end(); ++it) {
		//glDelete
		glDeleteTextures(1, &(*it).second);
		//ilDeleteImage((*it).second);
	}

	textureMap.clear();

	return true;
}