#pragma once

#include <Globals.h>

class Application;

class Module
{
public:

	Module()
	{
	}

	virtual ~Module() {

	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void WindowResized(unsigned newW, unsigned newH) {

	}
};
