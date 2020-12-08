#pragma once
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleEditorCamera.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleDebugDraw.h"
#include "Modules/ModuleTextures.h"
#include "Modules/ModuleScene.h"
#include <Timer.h>
#include <Leaks.h>


Application::Application()
{
	modules.reserve(8);
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(editorCamera = new ModuleEditorCamera());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(editor = new ModuleEditor());

	modules.push_back(scene = new ModuleScene());
	modules.push_back(renderer = new ModuleRender());

	modules.push_back(debugDraw = new ModuleDebugDraw());
	capTimer = new Timer();
	SetFrameCap(60);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		delete* it;
	}
}

float Application::GetDeltaTime() {
	return lastDeltaTime;
}

bool Application::Init()
{

	bool ret = true;
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

bool Application::Start()
{

	bool ret = true;
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}


update_status Application::Update()
{
	capTimer->Start();

	update_status ret = UPDATE_CONTINUE;


	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	if (millisPerFrame > (Uint32)0) {
		Uint32 frameMillis = capTimer->Read();

		if (frameMillis < millisPerFrame) {
			SDL_Delay(millisPerFrame - frameMillis);
		}
	}

	lastDeltaTime = capTimer->Read() / 1000;

	return ret;
}

void Application::WindowResized(unsigned newX, unsigned newY) {
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		(*it)->WindowResized(newX, newY);
	}
}

void Application::SetFrameCap(int newFrameCap) {
	frameCap = newFrameCap;

	millisPerFrame = frameCap > 0 ? 1000 / newFrameCap : 0;
}

const int& Application::GetFrameCap() const {
	return frameCap;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();


	delete capTimer;

	return ret;
}
