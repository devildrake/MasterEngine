#include "GameObjectHierarchyWindow.h"
#include "../Modules/ModuleScene.h"
#include "../GameObject.h"
#include "../Modules/ModuleInput.h"
#include "../Modules/ModuleEditor.h"
#include "../Modules/ModuleEditorCamera.h"
#include "../ImGuiWindows/PropertiesWindow.h"
#include "../Application.h"
#include "../Utilities/Leaks.h"
#include <Brofiler.h>

GameObject* selectedNode;
GameObject* movedGameObject;


GameObjectHierarchyWindow::GameObjectHierarchyWindow(const char* windowName, int id, ModuleScene* scene) :ImGuiWindow(windowName, id), currentScene(scene), prevSelectedNode(nullptr)/*, movedGameObject(nullptr)*/ {
	dragDropTargets = new std::pair<GameObject*, GameObject*>();
}

GameObjectHierarchyWindow::~GameObjectHierarchyWindow() {
	delete dragDropTargets;
}

void GameObjectHierarchyWindow::Draw() {

	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
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
					currentScene->CreateGameObject("GameObject", selectedNode != nullptr ? selectedNode : currentScene->GetRoot());
				}
				ImGui::EndPopup();
			}

			DrawChildren(currentScene->GetRoot(), false);
			if (dragDropTargets->first != nullptr) {
				OnDroppedObjectOverOther();
			}


			if (selectedNode != nullptr) {

				if (ImGui::BeginPopupContextWindow()) {
					if (ImGui::Selectable("Destroy")) {
						currentScene->DestroyGameObject(selectedNode);
						selectedNode = nullptr;
					}
					ImGui::EndPopup();
				}

				if (selectedNode != prevSelectedNode) {
					prevSelectedNode = selectedNode;
					App->editor->SetTargetObject(selectedNode);
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
//std::pair<int, int> GameObjectHierarchyWindow::DrawChildren(GameObject* target) const {
//	std::pair<int, int>ret(-1, -1);
//	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
//	const int& id = target->GetID();
//	const bool is_selected = (selection_mask & (1 << id)) != 0;
//	if (is_selected)
//		node_flags |= ImGuiTreeNodeFlags_Selected;
//
//	if (target->children.size() > 0) {
//
//		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, target->name.c_str(), id);
//
//
//
//		if (ImGui::IsItemClicked(ImGuiMouseButton(0)) || ImGui::IsItemClicked(ImGuiMouseButton(1))) {
//			nodeClicked = id;
//		}
//		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
//			ImGui::SetDragDropPayload("GOBJ", &id, sizeof(int));
//			//ImGui::Text("This is a drag and drop source");
//			ImGui::EndDragDropSource();
//		}
//		if (ImGui::BeginDragDropTarget()) {
//
//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {
//
//				int payload_n = *(const int*)payload->Data;
//
//				ret = std::pair<int, int>(payload_n, id);
//			}
//			ImGui::EndDragDropTarget();
//		}
//
//
//		if (node_open) {
//
//			for (std::list<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
//				//hierarchyID++;
//
//				if (ret.first == -1) {
//					ret = DrawChildren(*it);
//				}
//				else {
//					DrawChildren(*it);
//				}
//
//			}
//
//			ImGui::TreePop();
//		}
//	}
//	else {
//
//		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
//		ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, target->name.c_str());
//
//		if (ImGui::IsItemClicked(ImGuiMouseButton(0)) || ImGui::IsItemClicked(ImGuiMouseButton(1))) {
//			nodeClicked = id;
//		}
//		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
//			ImGui::SetDragDropPayload("GOBJ", &id, sizeof(int));
//			//ImGui::Text("This is a drag and drop source");
//			ImGui::EndDragDropSource();
//		}
//		if (ImGui::BeginDragDropTarget()) {
//
//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {
//
//				int payload_n = *(const int*)payload->Data;
//				ret = std::pair<int, int>(payload_n, id);
//			}
//			ImGui::EndDragDropTarget();
//		}
//	}
//	return ret;
//}



void GameObjectHierarchyWindow::DrawChildren(GameObject* target, bool drawSelf) const {
	dragDropTargets->first = dragDropTargets->second = nullptr;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	const bool is_selected = selectedNode == target;
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	bool node_open = false;
	if (target->children.size() > 0) {
		if (drawSelf) {
			node_open = ImGui::TreeNodeEx((void*)(intptr_t)target, node_flags, target->name.c_str());

			if (ImGui::IsItemClicked(ImGuiMouseButton(0))) {
				selectedNode = target;
				if (ImGui::IsMouseDoubleClicked(0)) {
					ComponentTransform* transform = (ComponentTransform*)selectedNode->GetComponentOfType(Component::ComponentType::CTTransformation);
					if (transform != nullptr) {
						if (App->editorCamera != nullptr) {
							App->editorCamera->FocusOn(transform);
						}
					}
				}
			} else if (ImGui::IsItemClicked(ImGuiMouseButton(1))) {
				selectedNode = target;
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("GOBJ", &selectedNode, sizeof(GameObject*));
				movedGameObject = target;
				//ImGui::Text("This is a drag and drop source");
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {

					//movedGameObject->SetNewParent(target);
					dragDropTargets->first = movedGameObject;
					dragDropTargets->second = target;
					LOG("tryna move %s into %s", movedGameObject->name.c_str(), target->name.c_str());
				}
				ImGui::EndDragDropTarget();
			}

		}
		if (node_open) {
			for (std::vector<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
				if (dragDropTargets->first == nullptr) {
					DrawChildren(*it);
				}
			}
			ImGui::TreePop();
		} else if (!drawSelf) {
			for (std::vector<GameObject*>::iterator it = target->children.begin(); it != target->children.end(); ++it) {
				if (dragDropTargets->first == nullptr) {
					DrawChildren(*it);
				}
			}
		}

	} else {

		if (!drawSelf)return;

		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)target, node_flags, target->name.c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton(0))) {
			selectedNode = target;
			if (ImGui::IsMouseDoubleClicked(0)) {
				ComponentTransform* transform = (ComponentTransform*)selectedNode->GetComponentOfType(Component::ComponentType::CTTransformation);
				if (transform != nullptr) {
					if (App->editorCamera != nullptr) {
						App->editorCamera->FocusOn(transform);
					}
				}
			}
		} else if (ImGui::IsItemClicked(ImGuiMouseButton(1))) {
			selectedNode = target;
		}


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("GOBJ", &selectedNode, sizeof(GameObject*));
			movedGameObject = target;
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GOBJ")) {
				//movedGameObject->SetNewParent(target);
				dragDropTargets->first = movedGameObject;
				dragDropTargets->second = target;
			}
			ImGui::EndDragDropTarget();
		}
	}
}


void GameObjectHierarchyWindow::OnDroppedObjectOverOther() {
	if (dragDropTargets->first != nullptr) {
		dragDropTargets->first->SetNewParent(dragDropTargets->second);
	}
}
