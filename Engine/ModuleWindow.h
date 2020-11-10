#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

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

	void SetFullScreen(bool b);
	void SetResizable(bool b);
	void SetBorderless(bool b);
	void SetFullDesktop(bool b);
	void WindowResized(unsigned width, unsigned height);
	void SetBrightness(float newB);
public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
private:
	int width, height;
	bool fullscreen, resizable, borderless, fullDtp;
	friend class ModuleEditor;
	float brightness;
};

#endif // __ModuleWindow_H__