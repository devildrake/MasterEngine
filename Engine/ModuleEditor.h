#ifndef MODULE_EDITOR_H
#define MODULE_EDITOR_H
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <vector>
#include <string>

#define FRAMECOUNT 30

class ModuleEditor :public Module {
private:
	bool showConsole;
	bool showConfig;
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	struct ConsoleWindow {
	private:
		std::vector<std::string>msgs;


	public:

		void Log(const char* msg) {
			msgs.push_back(msg);
		}

		void Draw(const char* consoleName, bool* p_open) {
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(consoleName, p_open))
			{
				ImGui::End();
				return;
			}

			for (int i = 0; i < msgs.size(); i++)
			{
				const char* item = msgs[i].c_str();


				// Normally you would store more information in your item than just a string.
				// (e.g. make Items[] an array of structure, store color/type etc.)
				ImVec4 color;
				bool has_color = false;
				if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
				else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
				if (has_color)
					ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(item);
				if (has_color)
					ImGui::PopStyleColor();
			}


			ImGui::End();
		}
	};
	struct ConfigMenu {
		float frames[FRAMECOUNT];
		float times[FRAMECOUNT];
		int frameCounter;
		ConfigMenu() {
			for (int i = 0; i < FRAMECOUNT; i++) {
				times[i] = 0;
				frames[i] = 0;
			}
			frameCounter = 0;
		}

		void AddFrame(float deltaTime) {
			times[frameCounter] = deltaTime * 1000;
			frames[frameCounter] = 1 / deltaTime;
			frameCounter = (frameCounter + 1) % FRAMECOUNT;
		}

		void Draw(const char* menuName, bool* p_open) {
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(menuName, p_open))
			{
				ImGui::End();
				return;
			}
			ImGui::Text("Options");


			if (ImGui::CollapsingHeader("Application")) {

				ImGui::Text("Options");
				//const float values[60];

				//ImVec2 size = ImGui::GetItemRectSize();

				char title[25];
				sprintf_s(title, 25, "Framerate %.1f", frames[FRAMECOUNT - 1]);
				ImGui::PlotHistogram("##framerate", frames, IM_ARRAYSIZE(frames), 0, NULL, 0.0f, 100.0f, ImVec2(310, 100));
				ImGui::PlotHistogram("##framerate", times, IM_ARRAYSIZE(times), 0, NULL, 0.0f, 32, ImVec2(310, 100));




			}






			ImGui::Text("Window");


			ImGui::End();
		}
	};
	ConfigMenu configMenu;
	ConsoleWindow console;


};

#endif