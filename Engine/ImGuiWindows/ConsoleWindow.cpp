#include "ConsoleWindow.h"
#include "../Application.h"
#include "../Modules/ModuleWindow.h"
#include <Leaks.h>
ConsoleWindow::ConsoleWindow(const char* aConsoleName) : ImGuiWindow(aConsoleName), doScroll(false), autoScroll(true) {

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
	ImGui::SetNextWindowSize(ImVec2((float)App->window->GetWidth(), (float)App->window->GetHeight() / 8.0f * 3.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((float)App->window->GetWidth() / 64.0f * 11.0f, (float)App->window->GetHeight() - App->window->GetHeight() / 8.0f * 2.0f), ImGuiCond_FirstUseEver);

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
