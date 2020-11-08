#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "ModuleEditor.h"
using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(input = new ModuleInput());
	modules.push_back(editorCamera = new ModuleEditorCamera());
	modules.push_back(editor = new ModuleEditor());

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
