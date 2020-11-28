#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H
#include "ImGuiWindow.h"
#include <vector>
#include <string>
#include <SDL.h>

#define FRAMECOUNT 60


class ConfigWindow :public ImGuiWindow {
private:
	std::pair< ImVec2, ImVec2> bounds;
	bool applicationHeaderActive, windowHeaderActive, hardwareHeaderActive, cameraHeaderActive;
	float frames[FRAMECOUNT];
	float times[FRAMECOUNT];
	int frameCounter;
	int refresh_rate;
	std::string caps;
public:
	ConfigWindow(const char* windowName);
	~ConfigWindow();
	void Draw();
	std::string GetCapsString();
	void AddFrame(float deltaTime);
	friend class ModuleEditor;

};

#endif