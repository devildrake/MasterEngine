#include "ModuleTextures.h"

#include "IL/ilu.h"

ModuleTextures::ModuleTextures() {

}
ModuleTextures::~ModuleTextures() {

}

GLuint ModuleTextures::LoadTexture(std::string path) {

	if (textureMap[path] != NULL) {
		return textureMap[path];
	}

	ILuint newImageID;
	GLuint newTextureID;
	//ilInit(); /* Initialization of DevIL */
	ilGenImages(1, &newImageID); /* Generation of one image name */
	ilBindImage(newImageID); /* Binding of image name */
	success = ilLoadImage(path.c_str()); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			return false;
		}
		glGenTextures(1, &newTextureID); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, newTextureID); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */
		  //

		ILinfo info = ILinfo();
		iluGetImageInfo(&info);

		if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

		textureMap[path] = newTextureID;

	}
	else
	{
		/* Error occured */
		return false;
	}
	ilDeleteImages(1, &newImageID); /* Because we have already copied image data into texture data
	  we can release memory used by image. */
	return newTextureID;
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
		ilDeleteImage((*it).second);
	}

	textureMap.clear();

	return true;
}