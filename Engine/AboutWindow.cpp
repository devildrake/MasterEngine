#include "AboutWindow.h"
#include "glew.h"

AboutWindow::AboutWindow(const char* windowName) :ImGuiWindow(windowName) {
	SDL_GetVersion(&sdl_version);
	imgui_version = ImGui::GetVersion();

	glew_version = (const char*)glewGetString(GLEW_VERSION);
}
AboutWindow::~AboutWindow() {

}
void AboutWindow::Draw() {
	if (!isOpen)return;

	if (!ImGui::Begin(windowName, &isOpen)) {
		ImGui::End();
		return;
	}

	static char engineNameBuff[32] = "Master Engine";
	static char descBuff[128] = "Engine made for the master's degree at UPC";
	static char authorBuff[32] = "By David Sierra";
	ImGui::InputText("Engine Name", engineNameBuff, IM_ARRAYSIZE(engineNameBuff));
	ImGui::InputText("Description", descBuff, IM_ARRAYSIZE(descBuff));
	ImGui::InputText("Author", authorBuff, IM_ARRAYSIZE(authorBuff));

	if (ImGui::CollapsingHeader("Libraries used")) {
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "Glew Version %s", glew_version);

		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "MathGeoLib v1.5");

		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "ImGui version %s", imgui_version);

		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "SDL version %d.%d.%d", sdl_version.major, sdl_version.major, sdl_version.patch);
	}

	ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "License: GNU General Public License v3.0");
	ImGui::End();
}