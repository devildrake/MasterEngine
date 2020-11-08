#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "MathGeoLib/Time/Clock.h"
#include "LTimer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditorCamera;
class ModuleEditor;
class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDeltaTime();
	void SetFrameCap(int frameCap);
	const int GetFrameCap()const;


public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditorCamera* editorCamera = nullptr;
	ModuleEditor* editor = nullptr;

private:
	int frameCap, ticksPerFrame;
	std::list<Module*> modules;
	LTimer capTimer;
	math::tick_t lastTick, newTick;

};

extern Application* App;
