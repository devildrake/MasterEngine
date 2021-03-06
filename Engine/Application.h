#pragma once

#include "Utilities/Globals.h"
#include "Modules/Module.h"
#include <vector>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleFileSystem;
class ModuleScene;
class Timer;

class Application {
public:

	Application();
	~Application();

	bool Init();
	bool Start();
	UpdateStatus Update();
	bool CleanUp();
	void MainWindowResized(unsigned, unsigned);
	//void SceneWindowResized(unsigned, unsigned);
	float GetDeltaTime();
	void SetFrameCap(int frameCap);
	const int& GetFrameCap()const;

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditorCamera* editorCamera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;
	ModuleScene* scene = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleFileSystem* fileSystem = nullptr;

private:
	unsigned int frameCap, millisPerFrame;
	float lastDeltaTime;
	std::vector<Module*> modules;
	Timer* capTimer;
};

extern Application* App;
