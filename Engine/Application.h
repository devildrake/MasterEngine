#pragma once

#include "Utilities/Globals.h"
#include "Modules/Module.h"
#include <Timer.h>
#include <list>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleScene;

//class ModuleHelloTriangleExcercise;
//class ModuleTransformedTriangleExcercise;
class ModuleTransformedTexturedTriangleExcercise;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	bool Start();
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
	ModuleDebugDraw* debugDraw = nullptr;
	ModuleScene* scene = nullptr;
	//ModuleHelloTriangleExcercise* triangleExcercise = nullptr;
	//ModuleTransformedTriangleExcercise* transformedTriangleExcercise = nullptr;
	//ModuleTransformedTexturedTriangleExcercise* transformedTexturedTriangleExcercise = nullptr;

	ModuleTextures* textures = nullptr;
private:
	Uint32 frameCap, millisPerFrame;
	float lastDeltaTime;
	std::list<Module*> modules;
	Timer capTimer;
};

extern Application* App;
