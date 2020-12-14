#include "ModuleFileSystem.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include <sys/stat.h>

ModuleFileSystem::ModuleFileSystem() :Module("ModuleFileSystem") {

}

ModuleFileSystem::~ModuleFileSystem() {

}

bool ModuleFileSystem::Init() {
	if (!IsDirectory("Resources")) {
		MakeDirectory("Resources");
	}

	if (!IsDirectory("Resources/Library")) {
		MakeDirectory("Resources/Library");
	}
	return true;
}

unsigned int ModuleFileSystem::Load(const char* path, const char* file, char** buffer)const {

	if (!IsDirectory(path)) {
		LOG("Error loading %s at %s (INVALID PATH)", file, path);
		return 0;
	} else {
		//TO DO ACTUAL LOADING
	}

	return 0;
}
unsigned int ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append)const {
	return 0;
}
bool ModuleFileSystem::Remove(const char* file) {
	return true;

}
bool ModuleFileSystem::Exists(const char* file)const {

	//return IsDirectory(file);
	return true;

}
bool ModuleFileSystem::MakeDirectory(const char* directory) {
	bool check = mkdir(directory) == 0;
	if (check) {
		LOG("directory %s created", directory);
	} else {
		LOG("directory %s NOT created", directory);
	}
	return check;
}
bool ModuleFileSystem::IsDirectory(const char* directory)const {
	struct stat buffer;

	bool check = stat(directory, &buffer) == 0;

	if (check) {
		LOG("directory %s found", directory);
	} else {
		LOG("directory %s NOT found", directory);
	}

	return check;
}
bool ModuleFileSystem::Copy(const char* source, const char* destintation) {
	return true;
}