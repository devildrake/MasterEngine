#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
#include "ModuleInput.h";
#include "ModuleDebugDraw.h"
#include "ModuleScene.h"

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
//#include "../EditorMainMenu.h"

#include <Leaks.h>

ModuleEditor::ModuleEditor() :Module("Editor"), currentTarget(nullptr), console(new ConsoleWindow("Console", 0)), frameCap(60.0f), configWindow(nullptr), propertiesWindow(nullptr), mainMenu(nullptr), aboutWindow(nullptr), hierarchyWindow(nullptr), gridMinSquares(-200), gridMaxSquares(200), gridPosY(0), gridStep(1.0f), gridColor(float3(0.5f, 0.5f, 0.5f)) {

}

ModuleEditor::~ModuleEditor() {
}

bool ModuleEditor::Init() {
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	return true;
}

bool ModuleEditor::Start() {
	if (!ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext())) {
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init()) {
		return false;
	}


	windows.push_back(console);
	windows.push_back(configWindow = new ConfigWindow("Configuration", 1));
	windows.push_back(propertiesWindow = new PropertiesWindow("Properties", 2));
	windows.push_back(aboutWindow = new AboutWindow("About", 3));
	windows.push_back(sceneWindow = new SceneWindow("Scene", 4));
	windows.push_back(hierarchyWindow = new GameObjectHierarchyWindow("Hierarchy", 5, App->scene));

	mainMenu = new EditorMainMenu(console->isOpen, configWindow->isOpen, propertiesWindow->isOpen, aboutWindow->isOpen, hierarchyWindow->isOpen);
	return true;
}

UpdateStatus ModuleEditor::PreUpdate() {

	configWindow->AddFrame(App->GetDeltaTime());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}
UpdateStatus ModuleEditor::Update() {

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

	return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleEditor::DrawMenu() {
	mainMenu->Draw();
}

UpdateStatus ModuleEditor::PostUpdate() {

	//ImGui::ShowDemoWindow();



	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	glClear(GL_COLOR_BUFFER_BIT);

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

	static bool dockSpace = true;
	if (ImGui::Begin("Docking", &dockSpace, windowFlags)) {
		DrawMenu();

		//App->editor->DrawMenu();

		ImGui::DockSpace(ImGui::GetID("Docking"));
		for (std::vector<ImGuiWindow*>::iterator it = windows.begin(); it != windows.end(); ++it) {
			(*it)->Draw();
		}

	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable) {
	//	SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
	//	SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
	//}

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


void ModuleEditor::SetTargetObject(GameObject* newTarget) {
	currentTarget = newTarget;
	App->editorCamera->SetTargetGameObject(newTarget);
}

GameObject* ModuleEditor::GetTargetObject()const {
	return currentTarget;
}

