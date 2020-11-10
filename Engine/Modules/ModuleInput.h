#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"

#include "../SDL/include/SDL_scancode.h"
#include "../MathGeoLib/Math/float3.h"
#include "../MathGeoLib/Math/float4.h"
#define NUM_MOUSE_BUTTONS 5

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

class ModuleInput : public Module
{

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
	const void MouseLeftWindow();
	void ResetMouseMotion();
	const bool IsMouseOverImGuiWindow()const;

private:
	bool		windowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	float3 mouse_motion;
	float3 mouse;
	float wheel_motion;
	bool mouseReset;
};

#endif // __MODULEINPUT_H__