#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H
#include "Module.h"

class Model;

class ModuleScene :public Module {
private:
	Model* currentModel;
public:
	ModuleScene();
	~ModuleScene();
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
public:
	friend class PropertiesWindow;
};

#endif