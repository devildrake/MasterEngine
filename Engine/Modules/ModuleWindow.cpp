#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleWindow.h"
#include "../Leaks.h"

ModuleWindow::ModuleWindow()
{
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	fullscreen = borderless = fullDtp = false;
	brightness = 1.0f;
	resizable = true;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		fullscreen = borderless = fullDtp = false;
		resizable = true;
		if (FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface

			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}


void ModuleWindow::SetFullScreen(bool b) {
	SDL_SetWindowFullscreen(window, b ? SDL_TRUE : SDL_FALSE);
	fullscreen = b;
}

void ModuleWindow::SetResizable(bool b) {
	SDL_SetWindowResizable(window, b ? SDL_TRUE : SDL_FALSE);
	resizable = b;
}

void ModuleWindow::SetBorderless(bool b) {
	SDL_SetWindowBordered(window, b ? SDL_FALSE : SDL_TRUE);
	borderless = b;
}

void ModuleWindow::SetFullDesktop(bool b) {
	//TODO
	fullDtp = b;
}


void ModuleWindow::WindowResized(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
}

void ModuleWindow::SetBrightness(float newB) {
	brightness = newB;
	SDL_SetWindowBrightness(window, newB);
}

const int ModuleWindow::GetWidth()const
{
	return width;
}
const int ModuleWindow::GetHeight()const {
	return height;
}