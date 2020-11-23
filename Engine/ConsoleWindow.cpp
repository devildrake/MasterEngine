#include "ConsoleWindow.h"
#include "Leaks.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
ConsoleWindow::ConsoleWindow(const char* aConsoleName) : ImGuiWindow(aConsoleName) {

}

void ConsoleWindow::Log(const char* msg) {
	msgs.push_back(msg);
}

ConsoleWindow::~ConsoleWindow() {
	msgs.clear();
	windowName = "";
	//windowName.clear();
}

void ConsoleWindow::Draw() {
	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth(), App->window->GetHeight() / 8 * 3), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() / 64 * 11, App->window->GetHeight() - App->window->GetHeight() / 8 * 2), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(windowName, &isOpen))
	{
		ImGui::End();
		return;
	}

	if (ImGui::SmallButton("Clear Console")) {
		msgs.clear();
	}

	for (int i = 0; i < msgs.size(); i++)
	{
		const char* item = msgs[i].c_str();

		ImVec4 color;
		bool has_color = false;
		if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
		else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
		if (has_color)
			ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextUnformatted(item);
		if (has_color)
			ImGui::PopStyleColor();
	}


	ImGui::End();
}
