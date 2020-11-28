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

#include "../EditorMainMenu.h"
#include <Leaks.h>

ModuleEditor::ModuleEditor() :console(new ConsoleWindow("Console")), frameCap(60.0f), configWindow(nullptr), propertiesWindow(nullptr), mainMenu(nullptr),aboutWindow(nullptr), gridMinSquares(-200), gridMaxSquares(200), gridPosY(0), gridStep(1.0f), gridColor(float3(0.5f, 0.5f, 0.5f)) {

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
	mainMenu = new EditorMainMenu(console->isOpen, configWindow->isOpen, propertiesWindow->isOpen, aboutWindow->isOpen);

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
	}

	if (App->debugDraw != nullptr) {
		App->debugDraw->DrawGrid(gridMinSquares, gridMaxSquares, gridPosY, gridStep, gridColor);
		App->debugDraw->DrawAxisTriad();
	}
	console->Draw();
	configWindow->Draw();
	propertiesWindow->Draw();
	aboutWindow->Draw();
	return 	mainMenu->Draw();
}
update_status ModuleEditor::PostUpdate() {

	ImGui::Render();
	//ImGuiContext handling

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
	}

	return UPDATE_CONTINUE;

}
bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete console;
	delete configWindow;
	delete propertiesWindow;
	delete aboutWindow;
	delete mainMenu;
	console = nullptr;
	configWindow = nullptr;
	mainMenu = nullptr;
	propertiesWindow = nullptr;
	return true;
}

ConsoleWindow* ModuleEditor::GetConsole() {
	return console;
}
