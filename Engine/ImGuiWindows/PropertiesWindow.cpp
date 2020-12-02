#include "PropertiesWindow.h"
#include "../Application.h"
#include "../Modules/ModuleScene.h"
#include "../Modules/ModuleWindow.h"
#include "../Rendering/Model.h"
#include <glew.h>
#include <Leaks.h>

PropertiesWindow::PropertiesWindow(const char* windowName) :ImGuiWindow(windowName), triangleCount(0), textureSize(0) {

}

PropertiesWindow::~PropertiesWindow() {

}

void PropertiesWindow::Draw() {
	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - App->window->GetWidth() / 8, App->window->GetHeight() - App->window->GetHeight() / 8 * 3), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(windowName, &isOpen))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Properites")) {
			isOpen = false;
		}
		ImGui::EndPopup();
	}

	if (ImGui::CollapsingHeader("Transform")) {

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Reset")) {
				App->scene->currentModel->ResetTransform();
			}
			ImGui::EndPopup();
		}

		if (ImGui::DragFloat3("Position", App->scene->currentModel->transform.localPosition.ptr())) {}
		if (ImGui::DragFloat3("Rotation", App->scene->currentModel->transform.rotation.ptr())) {}
		if (ImGui::DragFloat3("Scale", App->scene->currentModel->transform.scale.ptr())) {

		}
	}

	if (ImGui::CollapsingHeader("Model Info")) {

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Current Model:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", App->scene->currentModel->GetFileName().c_str());

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Tris:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->GetTris());

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Vertices:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->GetVertices());
	}

	if (ImGui::CollapsingHeader("Texture Info")) {

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Texture Count:");
		ImGui::SameLine();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->materials.size());

		for (int i = 0; i < App->scene->currentModel->materials.size(); ++i)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Texture: %d ", i);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s ", App->scene->currentModel->materials[i].GetTextureName().c_str());

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Path:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %s", App->scene->currentModel->materials[i].GetTexturePath().c_str());

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Texture size:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "% d x% d", App->scene->currentModel->materials[i].GetTextureSize().first, App->scene->currentModel->materials[i].GetTextureSize().second);
		}
	}


	ImGui::End();

}