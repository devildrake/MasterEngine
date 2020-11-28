#include "EditorMainMenu.h"
#include "Modules/ModuleEditor.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
EditorMainMenu::EditorMainMenu(bool& consoleWindowOpen, bool& configWindowOpen, bool& propertiesWindowOpen, bool& aboutWindowOpen) : consoleOpen(&consoleWindowOpen), configOpen(&configWindowOpen), propertiesOpen(&propertiesWindowOpen), aboutOpen(&aboutWindowOpen) {

}


update_status EditorMainMenu::Draw() {

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	//ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("MainMenu")) {
			if (consoleOpen != nullptr)
				if (ImGui::MenuItem("Console Window", "ALT+Y", consoleOpen)) {}
			if (configOpen != nullptr)
				if (ImGui::MenuItem("Configuration Window", "ALT+U", configOpen)) {}
			if (propertiesOpen != nullptr)
				if (ImGui::MenuItem("Properties Window", "ALT+P", propertiesOpen)) {}

#ifdef _WIN32
			if (ImGui::MenuItem("Open Github repository", "")) {
				ModuleEditor::open_url("https://github.com/devildrake/MasterEngine");
			}
#endif

			if (aboutOpen != nullptr)
				if (ImGui::MenuItem("About Window", "ALT + A", aboutOpen)) {}
			if (App->window->fullscreen) {
				if (ImGui::MenuItem("Exit FullScreen", "ALT + F")) {
					App->window->fullscreen = !App->window->fullscreen;
					App->window->SetFlag(SDL_WINDOW_FULLSCREEN);
				}
			}

			if (ImGui::MenuItem("Exit", "ALT + F4")) {
				return UPDATE_STOP;
			}

			//if(ImGui::)
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
		return UPDATE_CONTINUE;
	}
}