#include "ConsoleWindow.h"
#include "../Application.h"
#include "../Modules/ModuleWindow.h"
#include <Leaks.h>
ConsoleWindow::ConsoleWindow(const char* aConsoleName, int id) : ImGuiWindow(aConsoleName,id), doScroll(false), autoScroll(true) {

}

void ConsoleWindow::Log(const char* msg) {
	msgs.push_back(msg);
}

ConsoleWindow::~ConsoleWindow() {
	msgs.clear();
	windowName = "";
}

void ConsoleWindow::Draw()
{
	if (!isOpen)return;

	if (!ImGui::Begin(windowName, &isOpen)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Console")) {
			isOpen = false;
		}
		ImGui::EndPopup();
	}

	if (ImGui::SmallButton("Scroll Down")) {
		doScroll = true;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear")) {
		msgs.clear();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &autoScroll);

	ImGui::Separator();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) msgs.clear();
		ImGui::EndPopup();
	}

	for (int i = 0; i < msgs.size(); i++)
	{
		const char* item = msgs[i].c_str();

		ImVec4 color;
		bool has_color = false;
		if (strstr(item, "[error]")) {
			color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true;
		}
		else if (strncmp(item, "# ", 2) == 0) {
			color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true;
		}
		if (has_color) {
			ImGui::PushStyleColor(ImGuiCol_Text, color);
		}
		ImGui::TextUnformatted(item);
		if (has_color) {
			ImGui::PopStyleColor();
		}
	}
	if (doScroll || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
		ImGui::SetScrollHereY(1.0f);
	}
	doScroll = false;

	ImGui::EndChild();

	ImGui::End();
}
