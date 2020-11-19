#include "ConsoleWindow.h"
#include "Leaks.h"

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
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(windowName, &isOpen))
	{
		ImGui::End();
		return;
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
