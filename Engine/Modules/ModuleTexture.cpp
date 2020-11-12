#include "ModuleTexture.h"

#include "IL/ilu.h"

ModuleTexture::ModuleTexture() {

}
ModuleTexture::~ModuleTexture() {

}

GLuint ModuleTexture::LoadTexture(const char* path) {

	if (textureMap[path] != NULL) {
		return textureMap[path];
	}

	ILuint newTexId;
	GLuint newImage;
	ilInit(); /* Initialization of DevIL */
	ilGenImages(1, &newTexId); /* Generation of one image name */
	ilBindImage(newTexId); /* Binding of image name */
	success = ilLoadImage(path); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			return false;
		}
		glGenTextures(1, &newImage); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, newImage); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

		textureMap[path] = newImage;

	}
	else
	{
		/* Error occured */
		return false;
	}
	ilDeleteImages(1, &newTexId); /* Because we have already copied image data into texture data
	  we can release memory used by image. */
}

bool ModuleTexture::Init() {

	ilInit(); /* Initialization of DevIL */
	ilGenImages(1, &texid); /* Generation of one image name */
	ilBindImage(texid); /* Binding of image name */
	success = ilLoadImage("Lenna.png"); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			SDL_Quit();
			return -1;
		}
		glGenTextures(1, &image); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */

		  //glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		  //	ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		  //	ilGetData()); /* Texture specification */

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}


		//ILinfo* info = new ILinfo();
		//iluGetImageInfo(info);
		//if (info->Origin == IL_ORIGIN_LOWER_LEFT) {
		//	LOG("FUCKB");
		//}
		//else if (info->Origin == IL_ORIGIN_UPPER_LEFT) {

		//	iluFlipImage();
		//}


		//delete info;

	}
	else
	{
		/* Error occured */
		SDL_Quit();
		return false;
	}
	ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data
	  we can release memory used by image. */

	return true;
}

update_status ModuleTexture::PreUpdate() {
	return update_status::UPDATE_CONTINUE;
}


update_status ModuleTexture::Update() {

	//glTexCoord2i(0, 0); glVertex2i(0, 0);


	return update_status::UPDATE_CONTINUE;
}
update_status ModuleTexture::PostUpdate() {
	return update_status::UPDATE_CONTINUE;
}

bool  ModuleTexture::CleanUp() {
	ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data
   we can release memory used by image. */
	return true;
}