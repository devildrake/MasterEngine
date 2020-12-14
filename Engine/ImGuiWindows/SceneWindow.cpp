#include "SceneWindow.h"
#include "../Modules/ModuleRender.h"
#include "../Modules/ModuleWindow.h"
#include "../Modules/ModuleInput.h"
#include "../Application.h"
#include "../Utilities/Globals.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleEditorCamera.h"
SceneWindow::SceneWindow(const char* windowName, int id) :ImGuiWindow(windowName, id) {
	isOpen = true;
}

SceneWindow::~SceneWindow() {

}


bool SceneWindow::IsMouseOverWindow()const {
	//float3 mousePos = App->input->GetMousePosition();
	//ImVec2 size = ImGui::GetWindowSize();
	//ImVec2 pos = ImGui::GetWindowPos();
	//pos.x -= size.x/2;
	//pos.y -= size.y/2;

	//LOG("%f > %f and < %f", mousePos.x, pos.x, pos.x + size.x);
	//LOG("%f > %f and < %f", mousePos.y, pos.y, pos.x + size.x);



	//if (mousePos.x > pos.x && mousePos.x < pos.x + size.x) {
	//	if (mousePos.y > pos.y && mousePos.y < pos.y + size.y) {
	//		return true;
	//	}
	//}
	//	return false;

	return true;
}



void SceneWindow::Draw() {
	if (!isOpen)return;

	//ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth(), App->window->GetHeight());

	//ImGuiWindowFlags_AlwaysAutoResize
	std::pair<int, int>windowPos = App->window->GetWindowPos();


	if (!ImGui::Begin(windowName, &isOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollWithMouse)) {//, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse)) {
		ImGui::End();
		return;
	}

	isHovered = ImGui::IsWindowHovered();

	ImVec2 size = ImGui::GetWindowSize();
	if (prevSize.x != size.x || prevSize.y != size.y) {
		prevSize.x = size.x;
		prevSize.y = size.y;
		App->editorCamera->SetAspectRatio(size.x / size.y);
		//App->MainWindowResized(size.x, size.y);
		//App->SceneWindowResized(size.x, size.y);
	}

	ImVec2 uv_min = ImVec2(0.0f, 1.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 0.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
	//ImVec2(App->window->GetWidth(), App->window->GetHeight()

	//ImGui::Image((void*)App->renderer->GetRenderTextureID(), ImVec2(App->window->GetWidth(), App->window->GetHeight()), uv_min, uv_max);//, tint_col, border_col);
	ImGui::Image((void*)App->renderer->GetRenderTextureID(), size, uv_min, uv_max, tint_col, border_col);

	ImGui::End();
}

ImVec2 SceneWindow::GetSize()const {
	return prevSize;
}

const bool& SceneWindow::IsHovered()const {
	return isHovered;
}

