#ifndef CONSOLE_WINDOW_H
#define CONSOLE_WINDOW_H
#include "ImGuiWindow.h"
#include <vector>
#include <string>

class ConsoleWindow :public ImGuiWindow {
private:
	std::vector<std::string>msgs;
	bool autoScroll, doScroll;
public:
	ConsoleWindow(const char* windowName);
	~ConsoleWindow();
	void Draw()override;
	void Log(const char* msg);
	friend class ModuleEditor;
};

#endif