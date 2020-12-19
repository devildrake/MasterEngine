#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include <SDL.h>

class ModuleWindow : public Module {
public:

	ModuleWindow();
	~ModuleWindow();
	bool Init();
	bool CleanUp() override;

	void MainWindowResized(unsigned width, unsigned height)override;
	void SetBrightness(float newB);

	int GetWidth()const;
	int GetHeight()const;
	std::pair<int, int>GetWindowPos()const;
	void SetFlag(SDL_WindowFlags flag);

	bool IsVsyncOn()const;
	void SetVSync(bool should);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:

	//width and height are integers and not unsigned because ImGui::sliderInt does not support unsigned
	int width, height;

	bool fullscreen, resizable, borderless, fullDtp;
	float brightness;

	friend class ConfigWindow;
	friend class EditorMainMenu;
};

#endif // __ModuleWindow_H__