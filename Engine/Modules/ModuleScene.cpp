#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "../Model.h"
#include "../Application.h"
ModuleScene::ModuleScene() :currentModel(nullptr) {
}
ModuleScene::~ModuleScene() {

}
bool ModuleScene::Init() {
	currentModel = new Model("BakerHouse.fbx");
	App->renderer->AddModel(currentModel);
	return true;
}
bool ModuleScene::Start() {

	//App->renderer->AddModel(currentModel);
	return true;
}

update_status ModuleScene::PreUpdate() {
	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update() {


	const char* lastFile = App->input->GetLastFileDroppedOnWindow();

	if (lastFile != nullptr) {

		if (currentModel->GetFileName() != lastFile) {

			Model* newModel = new Model();
			if (Model::SceneFound(lastFile)) {
				App->renderer->RemoveModel(currentModel);
				delete currentModel;
				if (newModel->Load(lastFile)) {
					currentModel = newModel;
					App->renderer->AddModel(currentModel);
				}
				else {
					currentModel = nullptr;
					delete newModel;
				}
			}
			else {
				delete newModel;
			}
		}
	}

	return UPDATE_CONTINUE;
}
update_status ModuleScene::PostUpdate() {
	return UPDATE_CONTINUE;
}
bool ModuleScene::CleanUp() {

	if (currentModel != nullptr) {
		App->renderer->RemoveModel(currentModel);
		delete currentModel;
		currentModel = nullptr;
	}

	return true;
}


//#include "../Application.h"
//#include "ModuleEditorCamera.h"
//#include "ModuleRender.h"
//#include "ModuleInput.h"
//
//ModuleScene::ModuleScene() :Module() {
//
//}
//ModuleScene::~ModuleScene() {
//
//}
//bool ModuleScene::Init() {
//	return true;
//}
//bool ModuleScene::Start() {
//	currentModel = new Model("BakerHouse.fbx");
//	//App->renderer->AddModel(currentModel);
//	return true;
//}
//update_status ModuleScene::PreUpdate() {
//	return UPDATE_CONTINUE;
//
//}
//update_status ModuleScene::Update() {
//	const char* lastFile = App->input->GetLastFileDroppedOnWindow();
//
//	if (lastFile != nullptr) {
//
//		if (currentModel->GetFileName() != lastFile) {
//
//			Model* newModel = new Model();
//			if (Model::SceneFound(lastFile)) {
//				App->renderer->RemoveModel(currentModel);
//				delete currentModel;
//				if (newModel->Load(lastFile)) {
//					currentModel = newModel;
//					App->renderer->AddModel(currentModel);
//				}
//				else {
//					currentModel = nullptr;
//					delete newModel;
//				}
//			}
//			else {
//				delete newModel;
//			}
//		}
//	}
//	return UPDATE_CONTINUE;
//
//}
//update_status ModuleScene::PostUpdate() {
//	return UPDATE_CONTINUE;
//}
//bool ModuleScene::CleanUp() {
//	return true;
//}