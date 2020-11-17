#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H
#include "ImGuiWindow.h"
#include <vector>
#include <string>

#define FRAMECOUNT 30


class ConfigWindow :public ImGuiWindow {
private:
	std::pair< ImVec2, ImVec2> bounds;
	bool active;
	bool dummyBool;
	float frames[FRAMECOUNT];
	float times[FRAMECOUNT];
	int frameCounter;

public:
	ConfigWindow(const char* windowName);
	~ConfigWindow();
	void Draw();
	std::string GetCapsString();
	void AddFrame(float deltaTime);

};

#endif