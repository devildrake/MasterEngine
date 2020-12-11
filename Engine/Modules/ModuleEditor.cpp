#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
#include "ModuleInput.h";
#include "ModuleDebugDraw.h"

#include "../Application.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imconfig.h"

#include "../ImGuiWindows/ConsoleWindow.h"
#include "../ImGuiWindows/ConfigWindow.h"
#include "../ImGuiWindows/PropertiesWindow.h"
#include "../ImGuiWindows/AboutWindow.h"
#include "../ImGuiWindows/SceneWindow.h"
#include "../ImGuiWindows/GameObjectHierarchyWindow.h"

#include "../GameObject.h"
#include "../EditorMainMenu.h"
#include <Leaks.h>

ModuleEditor::ModuleEditor() :Module("Editor"), currentTarget(nullptr), console(new ConsoleWindow("Console")), frameCap(60.0f), configWindow(nullptr), propertiesWindow(nullptr), mainMenu(nullptr), aboutWindow(nullptr), hierarchyWindow(nullptr), gridMinSquares(-200), gridMaxSquares(200), gridPosY(0), gridStep(1.0f), gridColor(float3(0.5f, 0.5f, 0.5f)) {

}

ModuleEditor::~ModuleEditor() {
}

bool ModuleEditor::Init() {
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	return true;
}

bool ModuleEditor::Start() {
	if (!ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext())) {
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init()) {
		return false;
	}

	configWindow = new ConfigWindow("Configuration");
	propertiesWindow = new PropertiesWindow("Properties");
	aboutWindow = new AboutWindow("About");
	sceneWindow = new SceneWindow("Scene");
	hierarchyWindow = new GameObjectHierarchyWindow("Hierarchy", App->scene);
	mainMenu = new EditorMainMenu(console->isOpen, configWindow->isOpen, propertiesWindow->isOpen, aboutWindow->isOpen, hierarchyWindow->isOpen);
	return true;
}

update_status ModuleEditor::PreUpdate() {

	configWindow->AddFrame(App->GetDeltaTime());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}
update_status ModuleEditor::Update() {

	if (App->input->GetKey(SDL_SCANCODE_LALT) == ModuleInput::KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RALT) == ModuleInput::KEY_REPEAT) {
		if (App->input->GetKey(SDL_SCANCODE_Y) == ModuleInput::KEY_DOWN) {
			console->isOpen = !console->isOpen;
		}
		if (App->input->GetKey(SDL_SCANCODE_P) == ModuleInput::KEY_DOWN) {
			propertiesWindow->isOpen = !propertiesWindow->isOpen;
		}
		if (App->input->GetKey(SDL_SCANCODE_U) == ModuleInput::KEY_DOWN) {
			configWindow->isOpen = !configWindow->isOpen;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == ModuleInput::KEY_DOWN) {
			aboutWindow->isOpen = !aboutWindow->isOpen;
		}
		if (App->input->GetKey(SDL_SCANCODE_H) == ModuleInput::KEY_DOWN) {
			hierarchyWindow->isOpen = !hierarchyWindow->isOpen;
		}

	}

	if (App->debugDraw != nullptr) {
		App->debugDraw->DrawGrid(gridMinSquares, gridMaxSquares, gridPosY, gridStep, gridColor);

	}

	DrawGizmos();

	console->Draw();
	configWindow->Draw();
	propertiesWindow->Draw();
	aboutWindow->Draw();
	hierarchyWindow->Draw();


	sceneWindow->Draw();
	//mainMenu->Draw();

	return update_status::UPDATE_CONTINUE;
}

void ModuleEditor::DrawMenu() {
	mainMenu->Draw();
}

update_status ModuleEditor::PostUpdate() {

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable) {
		SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
	}

	return UPDATE_CONTINUE;

}

void ModuleEditor::DrawGizmos() {
	if (currentTarget != nullptr) {
		currentTarget->DrawGizmos();
	}
}

bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	RELEASE(console);
	RELEASE(configWindow);
	RELEASE(propertiesWindow);
	RELEASE(aboutWindow);
	RELEASE(hierarchyWindow);
	RELEASE(sceneWindow);
	RELEASE(mainMenu);

	return true;
}

ConsoleWindow* ModuleEditor::GetConsole() const {
	return console;
}

PropertiesWindow* ModuleEditor::GetProperties() const {
	return propertiesWindow;
}


SceneWindow* ModuleEditor::GetScene() const {
	return sceneWindow;
}

void ModuleEditor::WindowFocused()const {
	if (sceneWindow != nullptr) {
		sceneWindow->WindowFocused();
	}
}

void ModuleEditor::SetTargetObject(GameObject* newTarget) {
	currentTarget = newTarget;
	App->editorCamera->SetTargetGameObject(newTarget);
}

GameObject* ModuleEditor::GetTargetObject()const {
	return currentTarget;
}

