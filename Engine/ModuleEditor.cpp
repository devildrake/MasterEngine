#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

ModuleEditor::ModuleEditor() {
	showConsole = showConfig = true;

	/*buf = new char[10];
	f = 0;*/
}

ModuleEditor::~ModuleEditor() {

}

bool ModuleEditor::Init() {
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());

	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleEditor::PreUpdate() {


	return UPDATE_CONTINUE;

}

void MySaveFunction() {
	LOG("JIBIRI");
}

update_status ModuleEditor::Update() {


	configMenu.AddFrame(App->GetDeltaTime());



	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//	ImGui::ShowDemoWindow();
	//ImGui::Text("Hello world %d", 123);
	//if (ImGui::Button("Save")) {
	//	MySaveFunction();
	//}

	//ImGui::InputText("string", buf, sizeof(buf));
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	console.Draw("Console", &showConsole);
	configMenu.Draw("Configuration Menu", &showConfig);


	ImGui::Render();
	//Context handling
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



	//ImGui::Context


	return UPDATE_CONTINUE;

}
update_status ModuleEditor::PostUpdate() {
	return UPDATE_CONTINUE;

}
bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}