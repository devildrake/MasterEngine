#ifndef MODULE_FILE_SYSTEM_H
#define MODULE_FILE_SYSTEM_H
#include "Module.h"

class ModuleFileSystem :public Module {
	ModuleFileSystem();
	~ModuleFileSystem();
	unsigned int Load(const char* path, const char* file, char** buffer)const;
	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false)const;
	bool Remove(const char* file);
	bool Exists(const char* file)const;
	bool MakeDirectory(const char* directory);
	bool IsDirectory(const char* file)const;
	bool Copy(const char* source, const char* destintation);
};

#endif