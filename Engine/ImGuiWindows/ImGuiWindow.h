#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H
#include "../ImGui/imgui.h"
#include <string>

class ImGuiWindow {
protected:
	bool isOpen;
	const char* windowName;
public:

	ImGuiWindow(const char* aName) :windowName(aName), isOpen(true) {

	}

	ImGuiID GetID() {

		return ImGui::GetWindowViewport()->ID;
	}

	virtual void Draw() {
		//if (!ImGui::Begin(windowName, &p_open)) {
		//	ImGui::End();
		//}
	}
};

#endif