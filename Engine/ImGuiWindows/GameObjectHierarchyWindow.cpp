#include "GameObjectHierarchyWindow.h"
#include "../Modules/ModuleScene.h"
#include "../GameObject.h"
#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"
#include "../ImGuiWindows/PropertiesWindow.h"
#include "../Application.h"
#include "../Utilities/Leaks.h"


int nodeClicked = -1;

GameObjectHierarchyWindow::GameObjectHierarchyWindow(const char* windowName, ModuleScene* scene) :ImGuiWindow(windowName), currentScene(scene), prevNodeClicked(-1) {
}

GameObjectHierarchyWindow::~GameObjectHierarchyWindow() {

}

static int selection_mask = (1 << 2);;


void GameObjectHierarchyWindow::Draw() {

	//ImGui::ShowDemoWindow();

	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	//ImGui::ShowDemoWindow();
	if (!ImGui::Begin(windowName, &isOpen)) {
		ImGui::End();
		return;
	}

	ImGui::Text("Hierarchy");




	if (currentScene != nullptr) {
		if (currentScene->GetRoot() != nullptr) {
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::Selectable("Create GameObject")) {
					currentScene->CreateGameObject("GameObject", nodeClicked > -1 ? currentScene->GetGameObjectWithID(nodeClicked) : currentScene->GetRoot());
				}
				ImGui::EndPopup();
			}

			std::pair<int, int>parentModifyingPair = DrawChildren(currentScene->GetRoot());

			if (parentModifyingPair.first > -1) {
				currentScene->GetGameObjectWithID(parentModifyingPair.first)->SetNewParent(currentScene->GetGameObjectWithID(parentModifyingPair.second));
			}

			if (nodeClicked != -1) {

				if (ImGui::BeginPopupContextWindow()) {
					if (ImGui::Selectable("Destroy")) {
						currentScene->DestroyGameObject(currentScene->GetGameObjectWithID(nodeClicked));
					}
					ImGui::EndPopup();
				}

				// Update selection state
				// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
				if (App->input->GetKey(SDL_SCANCODE_LCTRL) == ModuleInput::KEY_REPEAT)
					selection_mask ^= (1 << nodeClicked);          // CTRL+click to toggle
				else if (!(selection_mask & (1 << nodeClicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
					selection_mask = (1 << nodeClicked);           // Click to single-select

				if (nodeClicked != prevNodeClicked) {
					prevNodeClicked = nodeClicked;
					App->editor->SetTargetObject(currentScene->GetGameObjectWithID(nodeClicked));
				}
			}

			//End Scrolling Region
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

const bool GameObjectHierarchyWindow::IsMouseOverWindow()const {
	return false;
}


//This method recursively draws every GameObject on scene (starting from rootNode) on the hierarchy as nodetrees.
//it returns a pair of integers that represent the indexes of the gameobjects that the drag/drop feature has happened on
std::pair<int, int> GameObjectHierarchyWindow::DrawChildren(GameObject* target) const {
	std::pair<int, int>ret(-1, -1);
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	const int& id = target->GetID();
	const bool is_selected = (selection_mask & (1 << id)) != 0;
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (target->children.size() > 0) {

		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, target->name.c_str(), id);



		if (ImGui::IsItemClicked(ImGuiMouseButton(0)) || ImGui::IsItemClicked(ImGuiMouseButton(1))) {
			nodeClicked = id;
		}
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("GOBJ", &id, sizeof(int));
			//ImGui::Text("This is a drag and drop source");
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {

				int payload_n = *(const int*)payload->Data;

				ret = std::pair<int, int>(payload_n, id);
			}
			ImGui::EndDragDropTarget();
		}


		if (node_open) {

			for (std::list<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
				//id++;

				if (ret.first == -1) {
					ret = DrawChildren(*it);
				}
				else {
					DrawChildren(*it);
				}

			}

			ImGui::TreePop();
		}
	}
	else {

		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, target->name.c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton(0)) || ImGui::IsItemClicked(ImGuiMouseButton(1))) {
			nodeClicked = id;
		}
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("GOBJ", &id, sizeof(int));
			//ImGui::Text("This is a drag and drop source");
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {

				int payload_n = *(const int*)payload->Data;
				ret = std::pair<int, int>(payload_n, id);
			}
			ImGui::EndDragDropTarget();
		}
	}
	return ret;
}