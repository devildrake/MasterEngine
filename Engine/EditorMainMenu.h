#ifndef EDITOR_MAIN_MENU_H
#define EDITOR_MAIN_MENU_H

#include <Globals.h>
#include "ImGui/imgui.h"
#include <string>

class EditorMainMenu {
private:
	bool* consoleOpen = nullptr;
	bool* configOpen = nullptr;
	bool* propertiesOpen = nullptr;
	bool* aboutOpen = nullptr;
	bool* hierarchyOpen = nullptr;
public:
	EditorMainMenu(bool& consoleWindowOpen, bool& configWindowOpen, bool& propertiesWindowOpen, bool& aboutOpen, bool& hierarchyOpen);
	update_status Draw();
};

#endif