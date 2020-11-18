#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "../ImGui/imconfig.h"
#include "../Leaks.h"
#include "../ConsoleWindow.h"
#include "../ConfigWindow.h"
#include "../Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
//#include "ImGui/imconfig.h"
//
//ModuleEditor::ModuleEditor() : console(ConsoleWindow("Console")), configMenu(ConfigWindow("Configuration")) {
//	//console = new ConsoleWindow("Console");
//	frameCap = 60.0f;
//}

ModuleEditor::ModuleEditor() {
	console = new ConsoleWindow("Console");
	configMenu = new ConfigWindow("Configuration");
	frameCap = 60.0f;
}

ModuleEditor::~ModuleEditor() {
	//delete console;
}

bool ModuleEditor::Init() {
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	return true;
}

bool ModuleEditor::Start() {
	if (!ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext())) {
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init()) {
		return false;
	}
	return true;
}

update_status ModuleEditor::PreUpdate() {

	configMenu->AddFrame(App->GetDeltaTime());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update() {
	console->Draw();
	configMenu->Draw();
	//ImGui::ShowDemoWindow();
	return UPDATE_CONTINUE;

}
update_status ModuleEditor::PostUpdate() {

	ImGui::Render();
	//Context handling

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
	delete configMenu;
	console = nullptr;
	configMenu = nullptr;
	//console.~ConsoleWindow();
	return true;
}

ConsoleWindow* ModuleEditor::GetConsole() {
	return console;
}

//MyType const& MyClass::getMyType() const { return mMyType; }
