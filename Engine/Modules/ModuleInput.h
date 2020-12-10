#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"

#include <SDL_scancode.h>
#include "../MathGeoLib/Math/float3.h"
#include "../MathGeoLib/Math/float4.h"
#define NUM_MOUSE_BUTTONS 5

class ModuleInput : public Module
{
public:
	enum EventWindow
	{
		WE_QUIT = 0,
		WE_HIDE = 1,
		WE_SHOW = 2,
		WE_COUNT
	};

	enum KeyState
	{
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

private:
	bool windowEvents[WE_COUNT];
	std::string lastFileDroppedOnWindow;
	KeyState* keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	float3 mouse_motion;
	float3 mouse;
	float wheel_motion;

private:
	void WarpMouseIfOutOfWindow();

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate();
	update_status Update();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const float3& GetMouseMotion() const;
	const float3& GetMousePosition() const;
	const float& GetMouseWheelMotion()const;
	std::string GetLastFileDroppedOnWindow() const;
	void SetLastFileDroppedOnWindow(char* newF);

};

#endif // __MODULEINPUT_H__