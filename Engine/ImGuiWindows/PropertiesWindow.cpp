#include "PropertiesWindow.h"
#include "../Application.h"
#include "../Modules/ModuleScene.h"
#include "../Modules/ModuleWindow.h"
#include "../Modules/ModuleEditor.h"
//#include "../Rendering/Model.h"
#include <glew.h>
#include <Leaks.h>
#include "../GameObject.h"

PropertiesWindow::PropertiesWindow(const char* windowName) :ImGuiWindow(windowName){

}

PropertiesWindow::~PropertiesWindow() {

}


//Iterates all components on targetGameObject and calls DrawEditor on each of them.
void PropertiesWindow::Draw() {
	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - App->window->GetWidth() / 8, App->window->GetHeight() - App->window->GetHeight() / 8 * 3), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(windowName, &isOpen)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Properites")) {
			isOpen = false;
		}
		ImGui::EndPopup();
	}

	if (App->editor->GetTargetObject() != nullptr) {

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->editor->GetTargetObject()->name.c_str());

		for (std::vector<Component*>::iterator it = App->editor->GetTargetObject()->components.begin(); it != App->editor->GetTargetObject()->components.end(); ++it) {
			(*it)->DrawEditor();
		}
	}


	ImGui::End();

}