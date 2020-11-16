#include "../Utilities/Globals.h"
#include "../Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "../SDL/include/SDL.h"
#include "ModuleEditor.h"
#define MAX_KEYS 300

ModuleInput::ModuleInput()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	mouseReset = false;
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
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
	//SDL_SetWindowGrab(App->window->window, SDL_TRUE);
	return ret;
}

bool ModuleInput::Start() {
	return true;
}

update_status ModuleInput::PreUpdate() {
	SDL_Event sdlEvent;
	mouse_motion.x = 0;
	mouse_motion.y = 0;
	wheel_motion = 0;
	//std::string a = std::to_string(mouse.x) + " " + std::to_string(mouse.y);
	//LOG(a.c_str());

	//SDL_CaptureMouse(true);

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
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;

			return UPDATE_STOP;
		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
				App->window->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			}
			else if (sdlEvent.window.event == SDL_WINDOWEVENT_LEAVE) {
				//App->renderer->MouseLeftWindow();
				MouseLeftWindow();
			}
			else if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
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
			if (mouseReset) {
				mouseReset = false;
			}

			if (math::Abs(sdlEvent.motion.xrel) < 150)
			{
				mouse_motion.x = sdlEvent.motion.xrel;// / SCREEN_SIZE;
			}

			if (math::Abs(sdlEvent.motion.yrel) < 150)
			{
				mouse_motion.y = sdlEvent.motion.yrel;// / SCREEN_SIZE;
			}

			//LOG(std::to_string(sdlEvent.motion.xrel).c_str());
			mouse.x = sdlEvent.motion.x;// / SCREEN_SIZE;
			mouse.y = sdlEvent.motion.y;// / SCREEN_SIZE;
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


const void ModuleInput::MouseLeftWindow() {
	//LOG("MOUSE LEFT WINDOW");
	/*float screenMargin = 40.0f;
	float3 mousePos = App->input->GetMousePosition();

	if (mousePos.x >= SCREEN_WIDTH / 2 - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, screenMargin, mousePos.y);
	}
	else if (mousePos.x <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, SCREEN_WIDTH - screenMargin, mousePos.y);
	}

	if (mousePos.y >= SCREEN_HEIGHT / 2 - screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, screenMargin);

	}
	else if (mousePos.y <= screenMargin) {
		SDL_WarpMouseInWindow(App->window->window, mousePos.x, SCREEN_HEIGHT - screenMargin);
	}*/

	//LOG("OUT");

	//SDL_WarpMouseInWindow(App->window->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
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

void ModuleInput::ResetMouseMotion() {
	mouseReset = true;
	mouse_motion = float3(0, 0, 0);
	mouse = float3(0, 0, 0);
}

const bool ModuleInput::IsMouseOverImGuiWindow()const {
	return ImGui::GetIO().WantCaptureMouse;
}
