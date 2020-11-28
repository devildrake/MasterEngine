#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "../Rendering/Model.h"
#include "../Application.h"
#include "ModuleEditorCamera.h"
#include "ModuleTextures.h"

ModuleScene::ModuleScene() :currentModel(nullptr) {
}
ModuleScene::~ModuleScene() {

}
bool ModuleScene::Init() {
	return true;
}
bool ModuleScene::Start() {

	currentModel = new Model("Resources\\Models\\BakerHouse.fbx");
	App->renderer->AddModel(currentModel);
	App->editorCamera->SetTargetModel(currentModel);
	return true;
}

update_status ModuleScene::PreUpdate() {
	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update() {

	std::string lastFile = App->input->GetLastFileDroppedOnWindow();

	if (lastFile.size() > 0) {
		if (currentModel != nullptr) {
			if (currentModel->GetFileName() != lastFile) {
				Model* newModel = new Model();

				if (Model::SceneFound(lastFile.c_str())) {

					App->renderer->RemoveModel(currentModel);
					delete currentModel;
					if (newModel->Load(lastFile.c_str())) {
						currentModel = newModel;
						App->renderer->AddModel(currentModel);
						App->editorCamera->SetTargetModel(currentModel);
					}
					else {
						currentModel = nullptr;
						delete newModel;
					}
				}
				else {
					delete newModel;
					GLuint possibleTexture;
					std::pair<int, int>possibleSize;
					if (lastFile != currentModel->GetMaterials()[0].GetTexturePath()) {
						if (App->textures->LoadTexture(lastFile, possibleTexture, possibleSize)) {
							LOG("%s was found to be a texture-compatible file, loading as diffuse for currentModel", lastFile.c_str());
							currentModel->ReleaseTextures();

							int lastSlash = 0;

							for (int i = lastFile.size(); i > 0 && lastSlash == 0; i--)
							{
								if (lastFile[i] == '\\') {
									lastSlash = i;
								}
							}

							currentModel->LoadMaterial(lastFile.substr(lastSlash, lastFile.size() - 1).c_str(), lastFile, possibleTexture, possibleSize);
							//currentModel->LoadMaterial()
						}
					}


				}
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