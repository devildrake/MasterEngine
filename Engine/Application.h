#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "MathGeoLib/Time/Clock.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditorCamera;
class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDeltaTime();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditorCamera* editorCamera = nullptr;
	math::tick_t lastTick, newTick;

private:

	std::list<Module*> modules;

};

extern Application* App;
