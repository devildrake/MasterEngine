#ifndef MODULE_EDITOR_H
#define MODULE_EDITOR_H
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
#include <vector>
#include <string>

#define FRAMECOUNT 30
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

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
		std::pair< ImVec2, ImVec2> bounds;
		bool active;
		bool dummyBool;
		float frames[FRAMECOUNT];
		float times[FRAMECOUNT];
		int frameCounter;

		std::string GetCapsString() {
			std::string capsString = "";

			if (SDL_Has3DNow()) {
				capsString += "3DNow ";
			}

			if (SDL_HasAVX()) {
				capsString += "AVX ";
			}

			if (SDL_HasAVX2()) {
				capsString += "AVX2 ";
			}

			if (SDL_HasAltiVec()) {
				capsString += "AltiVec ";
			}

			if (SDL_HasMMX()) {
				capsString += "MMX ";
			}

			if (SDL_HasSSE()) {
				capsString += "SSE ";
			}

			if (SDL_HasSSE2()) {
				capsString += "SSE2 ";
			}

			if (SDL_HasSSE3()) {
				capsString += "SSE3 ";
			}

			if (SDL_HasSSE41()) {
				capsString += "SSE41 ";
			}

			if (SDL_HasSSE42()) {
				capsString += "SSE42 ";
			}
			return capsString;
		}

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

			bounds.first = ImGui::GetWindowContentRegionMin();
			bounds.second = ImGui::GetWindowContentRegionMax();

			if (!ImGui::Begin(menuName, p_open))
			{
				ImGui::End();
				return;
			}
			ImGui::Text("Options");

			if (ImGui::CollapsingHeader("Application")) {

				ImGui::Text("Options");

				static char appNameBuff[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
				static char organizationBuff[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
				//static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
				ImGui::InputText("App Name", appNameBuff, IM_ARRAYSIZE(appNameBuff));
				ImGui::InputText("Organization", organizationBuff, IM_ARRAYSIZE(organizationBuff));

				if (ImGui::SliderInt("Max Fps", &App->editor->frameCap, 30, 90)) {
					App->SetFrameCap(App->editor->frameCap);
				}
				//const float values[60];

				//ImVec2 size = ImGui::GetItemRectSize();

				char title[25];
				sprintf_s(title, 25, "Framerate %.1f", frames[FRAMECOUNT - 1]);

				ImGui::PlotHistogram("##framerate", frames, IM_ARRAYSIZE(frames), 0, NULL, 0.0f, 100.0f, ImVec2(310, 100));

				sprintf_s(title, 25, "Milliseconds %0.1f", frames[FRAMECOUNT - 1]);

				ImGui::PlotHistogram("##framerate", times, IM_ARRAYSIZE(times), 0, NULL, 0.0f, 32, ImVec2(310, 100));
			}


			if (ImGui::CollapsingHeader("Window")) {
				if (ImGui::Checkbox("Active", &active)) {
					//App->editor->SetConfigActive(active);
				}
				ImGui::Text("Icon: *default*");
				if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0, 1.0f)) {
					App->window->SetBrightness(App->window->brightness);
				}
				if (ImGui::SliderInt("Width", &App->window->width, 640, 1920)) {
					SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
					App->renderer->WindowResized(App->window->width, App->window->height);
				}
				if (ImGui::SliderInt("Height", &App->window->height, 480, 1080)) {
					SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
					App->renderer->WindowResized(App->window->width, App->window->height);
				}

				ImGui::Text("Refresh rate: ");
				ImGui::TextColored(ImVec4(255, 255, 0, 1), "%d", 59);

				if (ImGui::Checkbox("FullScreen", &App->window->fullscreen)) {
					App->window->SetFullScreen(App->window->fullscreen);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Resizable", &App->window->resizable)) {
					App->window->SetResizable(App->window->resizable);
				}

				if (ImGui::Checkbox("Borderless", &App->window->borderless)) {
					App->window->SetBorderless(App->window->borderless);
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Fulldesktop", &App->window->fullDtp)) {
					App->window->SetFullDesktop(App->window->fullDtp);
				}


			}

			if (ImGui::CollapsingHeader("Hardware")) {
				//TO DO ACTIVE BOX
				//SDL_version* version;
				//SDL_GetVersion(version);

				//ImGui::Text("Version:");
				//ImGui::SameLine();
				//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SDL %d.%d.%d", version->major,version->minor,version->patch);


				ImGui::Text("CPUs:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());

				ImGui::Text("System RAM:");
				ImGui::SameLine();
				float ram = SDL_GetSystemRAM() / 1000;
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Gb", ram);

				ImGui::Text("Caps:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), GetCapsString().c_str(), SDL_GetCPUCount());

				ImGui::Text("GPU:");
				ImGui::SameLine();
				char* vendor = reinterpret_cast<char*>((unsigned char*)glGetString(GL_VENDOR));
				char* version = reinterpret_cast<char*>((unsigned char*)glGetString(GL_VERSION));
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Vendor: %s Driver Version: %s", vendor, version);

				ImGui::Text("Brand:");
				ImGui::SameLine();

				char* vRamBrand = reinterpret_cast<char*>((unsigned char*)glGetString(GL_RENDERER));
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), vRamBrand);

				GLint total_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
					&total_mem_kb);

				GLint cur_avail_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
					&cur_avail_mem_kb);

				ImGui::Text("VRAM budget:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f Gb", (float)(total_mem_kb) / 1024 / 1024);


				ImGui::Text("VRAM usage:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f Gb", (float)((total_mem_kb - cur_avail_mem_kb)) / 1024 / 1024);


				ImGui::Text("VRAM available:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f Gb", (float)(cur_avail_mem_kb) / 1024 / 1024);


				ImGui::Text("VRAM reserved:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f Gb", 0.0f);


				//const GLubyte* vendor = glGetString(GL_VENDOR);
				//const GLubyte* version = glGetString(GL_VERSION);
				//const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

				/*
				SDL_GetCPUCacheLineSize();
				SDL_GetCPUCount();*/
			}



			if (ImGui::CollapsingHeader("Camera")) {
				//ImGui::Text("To Do");
				if (ImGui::Checkbox("Active", &active)) {

				}
				float3 dummyFront;
				dummyFront = App->editorCamera->GetFrustum()->Front();
				if (ImGui::InputFloat3("Front", dummyFront.ptr())) {
					//dummyFront.Normalize();
					//App->editorCamera->GetFrustum()->SetFront(dummyFront);
				}

				dummyFront = App->editorCamera->GetFrustum()->Up();
				if (ImGui::InputFloat3("Up", dummyFront.ptr())) {
					//dummyFront.Normalize();
					//App->editorCamera->GetFrustum()->SetUp(dummyFront);
				}
				float3 pos = App->editorCamera->GetFrustum()->Pos();
				if (ImGui::InputFloat3("Pos", pos.ptr())) {
					App->editorCamera->SetFrustumPos(pos);
				}



				//TO DO camera mov speed
				if (ImGui::InputFloat("Mov Speed", &App->editorCamera->cameraSpeed)) {

				}


				//TO DO camera rot speed
				if (ImGui::InputFloat("Rot Speed", &App->editorCamera->rotationSpeed)) {

				}

				//TO DO ZOOM SPEED
				if (ImGui::InputFloat("Zoom Speed", &App->editorCamera->zoomSpeed)) {

				}

				if (ImGui::Checkbox("Frustum Culling", &App->editorCamera->frustumCulling)) {
					//TO DO SET FRUSTUM CULLING
				}

				//TO DO NEAR PLANE
				if (ImGui::InputFloat("Near Plane", &App->editorCamera->nearPlaneDistance)) {
					App->editorCamera->SetNearPlane(App->editorCamera->nearPlaneDistance);
				}


				//TO DO FAR PLANE
				if (ImGui::InputFloat("Far Plane", &App->editorCamera->farPlaneDistance)) {
					App->editorCamera->SetFarPlane(App->editorCamera->farPlaneDistance);
				}

				//TO DO ASPECT RATIO
				if (ImGui::InputFloat("Aspect Ratio", &App->editorCamera->aspectRatio)) {
					App->editorCamera->SetAspectRatio(App->editorCamera->aspectRatio);
				}

				//TO DO BG COLOR
				if (ImGui::InputFloat3("BG Color", App->renderer->bgColor.ptr())) {

				}


				//TO DO CURRENT DUMMY -NOT ASSIGNED YELLOW TEXT-
				ImGui::Text("Current:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0, 1.0, 0, 1.0), "Not assigned");

				//TO DO BUTTON PICK ANOTHER
				if (ImGui::ColorButton("Select other", ImVec4(0.7f, 0.0f, 0.0f, 0.7f))) {

				}

				//TO DO ISACTIVE CAMERA BOX
				if (ImGui::Checkbox("Is Active Camera", &dummyBool)) {

				}


			}

			if (ImGui::CollapsingHeader("About")) {
				ImGui::Text("Current:");
				static char engineNameBuff[32] = "Master Engine";
				static char descBuff[128] = "Engine made for the master's degree at UPC";
				static char authorBuff[32] = "By David Sierra";
				//static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
				ImGui::InputText("Engine Name", engineNameBuff, IM_ARRAYSIZE(engineNameBuff));
				ImGui::InputText("Description", descBuff, IM_ARRAYSIZE(descBuff));
				ImGui::InputText("Author", authorBuff, IM_ARRAYSIZE(authorBuff));

				if (ImGui::CollapsingHeader("Libraries used")) {
					ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "Glew 2.1.0");
					ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "MathGeoLib v1.5");
					ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "ImGui Docking 1.79");
					ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "SDL 2.0");

				}

				ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "License: GNU General Public License v3.0");

			}

			ImGui::End();
		}
	};

	ConfigMenu configMenu;
	ConsoleWindow console;
public:
	int frameCap;

};

#endif