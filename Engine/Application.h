#pragma once

#include<list>
#include "Utilities/Globals.h"
#include "Modules/Module.h"
#include "MathGeoLib/Time/Clock.h"
#include "Utilities/LTimer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTexture;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleDebugDraw;


//class ModuleHelloTriangleExcercise;
//class ModuleTransformedTriangleExcercise;
class ModuleTransformedTexturedTriangleExcercise;

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
	ModuleDebugDraw* debugDraw = nullptr;
	//ModuleHelloTriangleExcercise* triangleExcercise = nullptr;
	//ModuleTransformedTriangleExcercise* transformedTriangleExcercise = nullptr;
	ModuleTransformedTexturedTriangleExcercise* transformedTexturedTriangleExcercise = nullptr;

	ModuleTexture* texture = nullptr;
private:
	int frameCap, ticksPerFrame;
	std::list<Module*> modules;
	LTimer capTimer;
	math::tick_t lastTick, newTick;

};

extern Application* App;
