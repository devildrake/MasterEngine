#include "SceneWindow.h"
#include "../Modules/ModuleRender.h"
#include "../Modules/ModuleWindow.h"
#include "../Modules/ModuleInput.h"
#include "../Application.h"

SceneWindow::SceneWindow(const char* windowName) :ImGuiWindow(windowName) {
	isOpen = true;
}

SceneWindow::~SceneWindow() {

}

bool SceneWindow::IsMouseOverWindow()const {
	float3 mousePos = App->input->GetMousePosition();
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();

	if (mousePos.x > pos.x && mousePos.x < pos.x + size.x) {
		if (mousePos.y > pos.y && mousePos.y < pos.y + size.y) {
			return true;
		}
	}
	return false;
}



void SceneWindow::Draw() {
	if (!isOpen)return;

	//ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth(), App->window->GetHeight());

	//ImGuiWindowFlags_AlwaysAutoResize
	if (!ImGui::Begin(windowName, &isOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse)) {
		ImGui::End();
		return;
	}



	ImVec2 size = ImGui::GetWindowSize();
	if (prevSize.first != size.x || prevSize.second != size.y) {
		prevSize.first = size.x;
		prevSize.second = size.y;
		App->WindowResized(size.x, size.y);
	}


	ImVec2 uv_min = ImVec2(0.0f, 1.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 0.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white



	ImGui::Image((void*)App->renderer->GetRenderTextureID(), ImVec2(App->window->GetWidth(), App->window->GetHeight()), uv_min, uv_max, tint_col, border_col);




	ImGui::End();
}

