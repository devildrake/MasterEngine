#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "../Utilities/Leaks.h"

ModuleWindow::ModuleWindow() :Module("Window"), fullscreen(false), borderless(false), fullDtp(false), resizable(true), brightness(1.0f), width(0), height(0) {}

// Destructor
ModuleWindow::~ModuleWindow() {
}

// Called before render is available
bool ModuleWindow::Init() {

	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		//Create window
		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm);

		width = dm.w / 8 * 6;
		height = dm.h / 8 * 6;

		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | !SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_INPUT_FOCUS;
		fullscreen = borderless = fullDtp = false;
		resizable = true;
		if (FULLSCREEN == true) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL) {
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else {
			//Get window surface

			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp() {
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}


void ModuleWindow::SetFlag(SDL_WindowFlags flag) {

	switch (flag) {
	case SDL_WINDOW_BORDERLESS:
		SDL_SetWindowBordered(window, borderless ? SDL_FALSE : SDL_TRUE);
		break;
	case SDL_WINDOW_FULLSCREEN:
		SDL_SetWindowFullscreen(window, fullscreen ? SDL_TRUE : SDL_FALSE);
		break;
	case SDL_WINDOW_RESIZABLE:
		SDL_SetWindowResizable(window, resizable ? SDL_TRUE : SDL_FALSE);
		break;
	case SDL_WINDOW_FULLSCREEN_DESKTOP:
		SDL_SetWindowFullscreen(window, fullscreen ? SDL_TRUE : SDL_FALSE);
		break;
	default:
		break;
	}

}



void ModuleWindow::MainWindowResized(unsigned width, unsigned height) {
	this->width = width;
	this->height = height;
}

void ModuleWindow::SetBrightness(float newB) {
	brightness = newB;
	SDL_SetWindowBrightness(window, newB);
}

int ModuleWindow::GetWidth()const {
	return width;
}
int ModuleWindow::GetHeight()const {
	return height;
}

std::pair<int, int> ModuleWindow::GetWindowPos()const {
	std::pair<int, int> ret;
	SDL_GetWindowPosition(window, &ret.first, &ret.second);
	return ret;
}