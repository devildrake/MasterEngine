#include "EditorMainMenu.h"
#include "Modules/ModuleEditor.h"
#include "SDL/include/SDL.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
EditorMainMenu::EditorMainMenu(bool* consoleWindowOpen, bool* configWindowOpen, bool* propertiesWindowOpen) : consoleOpen(consoleWindowOpen), configOpen(configWindowOpen), propertiesOpen(propertiesWindowOpen) {

}


update_status EditorMainMenu::Draw() {

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	//ImGui::DockSpaceOverViewport();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("MainMenu")) {
			if (consoleOpen != nullptr)
				if (ImGui::MenuItem("Console Window", "CTRL+Y", consoleOpen)) {}
			if (configOpen != nullptr)
				if (ImGui::MenuItem("Configuration Window", "CTRL+U", configOpen)) {}
			if (propertiesOpen != nullptr)
				if (ImGui::MenuItem("Properties Window", "CTRL+P", propertiesOpen)) {}

#ifdef _WIN32
			if (ImGui::MenuItem("Open Github repository", "")) {
				ModuleEditor::open_url("https://github.com/devildrake/MasterEngine");
			}
#endif


			if (ImGui::MenuItem("About", "")) {

				static char messageBuff[256] = "Master Engine\nEngine made for the master's degree at UPC\nBy David Sierra\nLibraries used:\nGlew 2.1.0\nMathGeoLib v1.5\nImGui Docking 1.79\nSDL 2.0\nLicense: GNU General Public License v3.0";


				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "About", messageBuff, App->window->window);
			}
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