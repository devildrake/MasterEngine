#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H
#include "ImGuiWindow.h"
#include <vector>
#include <string>
#include <SDL.h>

#define FRAMECOUNT 60


class ConfigWindow :public ImGuiWindow {
private:
	bool applicationHeaderActive, windowHeaderActive, hardwareHeaderActive, cameraHeaderActive;
	float frames[FRAMECOUNT];
	float times[FRAMECOUNT];
	int frameCounter;

	//Screen refresh rate
	int refresh_rate;

	//String with all GPU capabilities added to it as strings
	std::string caps;
public:
	ConfigWindow(const char* windowName, int id);
	~ConfigWindow();
	void Draw()override;
	std::string GetCapsString();
	void AddFrame(float deltaTime);
	friend class ModuleEditor;

};

#endif