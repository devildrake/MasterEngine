#include "PropertiesWindow.h"
#include "Application.h"
#include "Modules/ModuleScene.h"
#include "Model.h"
PropertiesWindow::PropertiesWindow(const char* windowName) :ImGuiWindow(windowName) {
	modelPosition = App->scene->currentModel->Position();
	modelScale = App->scene->currentModel->Scale();
	modelRotation = App->scene->currentModel->Rotation();
}
PropertiesWindow::~PropertiesWindow() {

}
void PropertiesWindow::Draw() {
	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(windowName, &isOpen))
	{
		ImGui::End();
		return;
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

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Current Model:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", App->scene->currentModel->GetFileName().c_str());

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Tris:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->GetTris());

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Vertices:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->GetVertices());

	//ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Textures:");
	//ImGui::SameLine();
	//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->scene->currentModel->




	ImGui::End();

}