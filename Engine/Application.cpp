#pragma once
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleEditorCamera.h"
#include "Modules/ModuleEditor.h"
//#include "Excercises/ModuleHelloTriangleExcerise.h"
//#include "Excercises/ModuleTransformedTriangleExcercise.h"
//#include "Excercises/ModuleTransformedTexturedTriangleExcercise.h"
#include "Modules/ModuleDebugDraw.h"
#include "Modules/ModuleTextures.h"
#include "Leaks.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(editorCamera = new ModuleEditorCamera());
	modules.push_back(textures = new ModuleTextures());
	//modules.push_back(transformedTexturedTriangleExcercise = new ModuleTransformedTexturedTriangleExcercise());
	modules.push_back(editor = new ModuleEditor());
	//modules.push_back(triangleExcercise = new ModuleHelloTriangleExcercise());
	//modules.push_back(transformedTriangleExcercise = new ModuleTransformedTriangleExcercise());
	modules.push_back(renderer = new ModuleRender());

	modules.push_back(debugDraw = new ModuleDebugDraw());
	lastTick = newTick = Clock::Tick();
	SetFrameCap(60);
}

Application::~Application()
{
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		delete* it;
	}
}

float Application::GetDeltaTime() {
	return Clock::TimespanToSecondsF(lastTick, newTick);
}

bool Application::Init()
{

	bool ret = true;
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

bool Application::Start()
{

	bool ret = true;
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}


update_status Application::Update()
{
	capTimer.start();

	lastTick = newTick;
	newTick = Clock::Tick();

	update_status ret = UPDATE_CONTINUE;


	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	int frameTicks = capTimer.getTicks();
	if (frameTicks < ticksPerFrame)
	{
		//Wait remaining time
		SDL_Delay(ticksPerFrame - frameTicks);
	}

	return ret;
}

void Application::SetFrameCap(int newFrameCap) {
	frameCap = newFrameCap;
	ticksPerFrame = 1000 / newFrameCap;
}



inline const int Application::GetFrameCap() const {
	return frameCap;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
