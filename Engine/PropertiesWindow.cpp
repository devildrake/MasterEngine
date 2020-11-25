#include "PropertiesWindow.h"
#include "Application.h"
#include "Modules/ModuleScene.h"
#include "Modules/ModuleWindow.h"
#include "Model.h"
#include "glew.h"
PropertiesWindow::PropertiesWindow(const char* windowName) :ImGuiWindow(windowName), modelPosition(0, 0, 0), modelScale(0, 0, 0), modelRotation(0, 0, 0), triangleCount(0), textureSize(0) {
	if (App->scene->currentModel != nullptr) {
		modelPosition = App->scene->currentModel->Position();
		modelScale = App->scene->currentModel->Scale();
		modelRotation = App->scene->currentModel->Rotation();
	}
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
		if (ImGui::DragFloat3("Position", modelPosition.ptr())) {
			App->scene->currentModel->SetPos(modelPosition);
		}
		if (ImGui::DragFloat3("Rotation", modelRotation.ptr())) {
			App->scene->currentModel->SetRotation(modelRotation);
		}

		if (ImGui::DragFloat3("Scale", modelScale.ptr())) {
			App->scene->currentModel->SetScale(modelScale);

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