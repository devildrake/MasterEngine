#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "../ImGui/imconfig.h"



//#include "ImGui/imconfig.h"

ModuleEditor::ModuleEditor() {
	showConsole = showConfig = true;
	frameCap = 60.0f;
}

ModuleEditor::~ModuleEditor() {

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

	configMenu.AddFrame(App->GetDeltaTime());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

void MySaveFunction() {
	LOG("JIBIRI");
}

update_status ModuleEditor::Update() {
	console.Draw("Console", &showConsole);
	configMenu.Draw("Configuration Menu", &showConfig);
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
	return true;
}