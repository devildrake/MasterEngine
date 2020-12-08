#include <Globals.h>
#include "../Application.h"
#include "ModuleInput.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleEditorCamera.h"
#include "../SDL/include/SDL.h"
#include "ModuleEditor.h"
#include <Leaks.h>

#define MAX_KEYS 300

ModuleInput::ModuleInput() : lastFileDroppedOnWindow("")

{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{

}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	return ret;
}

bool ModuleInput::Start() {
	return true;
}

std::string ModuleInput::GetLastFileDroppedOnWindow() const {
	return lastFileDroppedOnWindow;
}

void ModuleInput::SetLastFileDroppedOnWindow(char* newF) {
	lastFileDroppedOnWindow = newF;
}

update_status ModuleInput::PreUpdate() {
	SDL_Event sdlEvent;
	mouse_motion.x = 0;
	mouse_motion.y = 0;
	wheel_motion = 0;

	if (lastFileDroppedOnWindow.size() > 0) {
		//SDL_free(lastFileDroppedOnWindow.c_str());    // Free dropped_filedir memory
		//lastFileDroppedOnWindow = nullptr;
		lastFileDroppedOnWindow = "";
	}

	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}


	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);


		switch (sdlEvent.type)
		{
		case (SDL_DROPFILE): {
			//For one frame, upon dropping a file, its path will be stored so that other modules may access it and use it
			if (lastFileDroppedOnWindow.size() > 0) {
				//SDL_free(lastFileDroppedOnWindow);    // Free dropped_filedir memory
				//lastFileDroppedOnWindow = nullptr;
				lastFileDroppedOnWindow = "";
			}
			lastFileDroppedOnWindow = sdlEvent.drop.file;


			break;
		}
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			return UPDATE_STOP;
		case SDL_WINDOWEVENT:
			//if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {

			//	LOG("%d", sdlEvent.window.windowID);

			//	if (sdlEvent.window.windowID == App->editor->GetEditorWindowID()/*SDL_GetWindowID(App->window->window)*/) {
			//		if (App->renderer != nullptr) {
			//			App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			//		}
			//		if (App->window != nullptr) {
			//			App->window->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			//		}

			//		if (App->editorCamera != nullptr) {
			//			App->editorCamera->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			//		}
			//	}

			//}
			/*else*/ if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
				windowEvents[WE_QUIT] = true;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[sdlEvent.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[sdlEvent.button.button - 1] = KEY_UP;
			break;


		case SDL_MOUSEMOTION:

			if (math::Abs(sdlEvent.motion.xrel) < 150)
			{
				mouse_motion.x = sdlEvent.motion.xrel;
			}

			if (math::Abs(sdlEvent.motion.yrel) < 150)
			{
				mouse_motion.y = sdlEvent.motion.yrel;
			}

			//WarpMouseIfOutOfWindow();



			mouse.x = sdlEvent.motion.x;
			mouse.y = sdlEvent.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			wheel_motion = sdlEvent.wheel.y;
			break;
		}
	}

	if ((GetKey(SDL_SCANCODE_LALT) == KEY_DOWN && GetKey(SDL_SCANCODE_F4) == KEY_DOWN) || windowEvents[WE_QUIT] == true)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
	return UPDATE_CONTINUE;
}

void ModuleInput::WarpMouseIfOutOfWindow() {

	if (mouse.x >= (float)App->window->GetWidth()) {
		SDL_WarpMouseInWindow(App->window->window, 0, mouse.y);
	}
	else if (mouse.x < 0.0f) {
		SDL_WarpMouseInWindow(App->window->window, App->window->GetWidth() - 1, mouse.y);
	}

	if (mouse.y >= (float)App->window->GetHeight()) {
		SDL_WarpMouseInWindow(App->window->window, mouse.x, 0);

	}
	else if (mouse.y < 0.0f) {
		SDL_WarpMouseInWindow(App->window->window, mouse.x, App->window->GetHeight() - 1);
	}
}

// Called before quitting
bool ModuleInput::CleanUp()
{

	//if (lastFileDroppedOnWindow != nullptr) {
	//	SDL_free(lastFileDroppedOnWindow);    // Free dropped_filedir memory
	//}
	if (lastFileDroppedOnWindow.size() > 0) {
		lastFileDroppedOnWindow.clear();
	}

	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	RELEASE_ARRAY(keyboard);


	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const float& ModuleInput::GetMouseWheelMotion()const {
	return wheel_motion;
}

const float3& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const float3& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}


const bool ModuleInput::IsMouseOverImGuiWindow()const {
	return ImGui::GetIO().WantCaptureMouse;
}
