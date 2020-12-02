#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include <SDL.h>

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void WindowResized(unsigned width, unsigned height);
	void SetBrightness(float newB);
	const int GetWidth()const;
	const int GetHeight()const;
	void SetFlag(SDL_WindowFlags flag);
public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	int width, height;
	bool fullscreen, resizable, borderless, fullDtp;
	float brightness;

	friend class ConfigWindow;
	friend class EditorMainMenu;
};

#endif // __ModuleWindow_H__