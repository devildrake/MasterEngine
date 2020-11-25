#include "ConfigWindow.h"
#include "Utilities/Globals.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleEditorCamera.h"
#include "Modules/ModuleEditor.h"
#include "Application.h"
#include "glew.h"
#include "Leaks.h"

std::string ConfigWindow::GetCapsString() {
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

ConfigWindow::ConfigWindow(const char* aConfigName) : ImGuiWindow(aConfigName), hardwareHeaderActive(true), cameraHeaderActive(true), windowHeaderActive(true), applicationHeaderActive(true) {
	for (int i = 0; i < FRAMECOUNT; i++) {
		times[i] = 0;
		frames[i] = 0;
	}
	frameCounter = 0;
	SDL_GetVersion(&sdl_version);
	imgui_version = ImGui::GetVersion();
	glew_version = (const char*)glewGetString(GLEW_VERSION);

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	refresh_rate = mode.refresh_rate;
	caps = GetCapsString();
}


ConfigWindow::~ConfigWindow() {
	windowName = "";
}

void ConfigWindow::AddFrame(float deltaTime) {
	times[frameCounter] = deltaTime * 1000;
	frames[frameCounter] = 1 / deltaTime;
	frameCounter = (frameCounter + 1) % FRAMECOUNT;
}

void ConfigWindow::Draw() {
	if (!isOpen)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	bounds.first = ImGui::GetWindowContentRegionMin();
	bounds.second = ImGui::GetWindowContentRegionMax();

	if (!ImGui::Begin(windowName, &isOpen))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Window")) {
			isOpen = false;
		}
		ImGui::EndPopup();
	}

	ImGui::Text("Options");


	if (ImGui::CollapsingHeader("Application")) {
		if (ImGui::Checkbox("Active", &applicationHeaderActive)) {}

		if (applicationHeaderActive) {
			ImGui::Text("Options");

			static char appNameBuff[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
			static char organizationBuff[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
			ImGui::InputText("App Name", appNameBuff, IM_ARRAYSIZE(appNameBuff));
			ImGui::InputText("Organization", organizationBuff, IM_ARRAYSIZE(organizationBuff));

			if (ImGui::SliderInt("Max Fps", &App->editor->frameCap, 30, 90)) {
				App->SetFrameCap(App->editor->frameCap);
			}

			ImGui::PlotHistogram("##framerate", frames, IM_ARRAYSIZE(frames), 0, NULL, 0.0f, 100.0f, ImVec2(310, 100));
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "FrameRate");

			ImGui::PlotHistogram("##times", times, IM_ARRAYSIZE(times), 0, NULL, 0.0f, 32, ImVec2(310, 100));
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Times");

		}
	}

	if (ImGui::CollapsingHeader("Window")) {
		if (ImGui::Checkbox("Active", &windowHeaderActive)) {}
		if (windowHeaderActive) {
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
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 1), "%d", refresh_rate);

			if (ImGui::Checkbox("FullScreen", &App->window->fullscreen)) {
				App->window->SetFlag(SDL_WINDOW_FULLSCREEN);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable)) {
				App->window->SetFlag(SDL_WINDOW_RESIZABLE);
			}

			if (ImGui::Checkbox("Borderless", &App->window->borderless)) {
				App->window->SetFlag(SDL_WINDOW_BORDERLESS);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Fulldesktop", &App->window->fullscreen)) {
				App->window->SetFlag(SDL_WINDOW_FULLSCREEN_DESKTOP);
			}


		}
	}

	if (ImGui::CollapsingHeader("Hardware")) {
		if (ImGui::Checkbox("Active", &hardwareHeaderActive)) {}
		if (hardwareHeaderActive) {

			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());

			ImGui::Text("System RAM:");
			ImGui::SameLine();
			float ram = SDL_GetSystemRAM() / 1000;
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f Gb", ram);

			ImGui::Text("Caps:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), caps.c_str(), SDL_GetCPUCount());

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


			//ImGui::Text("VRAM reserved:");
			//ImGui::SameLine();
			//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f Gb", (float)(reserved_mem_kb) / 1024 / 1024);

		}
	}


	if (App->editorCamera != nullptr) {
		if (ImGui::CollapsingHeader("Camera")) {
			//ImGui::Text("To Do");
			if (ImGui::Checkbox("Active", &cameraHeaderActive)) {}

			if (cameraHeaderActive) {
				// Front and Up are READ ONLY values, thus the dummy float3
				float3 dummyFront = App->editorCamera->GetFrustum()->Front();
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

				ImGui::DragFloat("Camera Speed", &App->editorCamera->cameraSpeed, 0.1f, 0.5f, 10.0f);
				ImGui::DragFloat("Camera Rot Speed", &App->editorCamera->rotationSpeed, 0.1f, 15.0f, 60.0f);
				ImGui::DragFloat("Camera Zoom Speed", &App->editorCamera->zoomSpeed, 5.0f, 0.1f, 60.0f);
				ImGui::DragFloat("Camera Focus distance", &App->editorCamera->focusDistance, 0.1f, 0.1f, 4.0f);
				ImGui::DragFloat("Camera Orbit Speed", &App->editorCamera->orbitSpeed, 0.1f, 0.1f, 12.0f);

				if (ImGui::InputFloat("Near Plane", &App->editorCamera->nearPlaneDistance)) {
					App->editorCamera->SetNearPlane(App->editorCamera->nearPlaneDistance);
				}

				if (ImGui::InputFloat("Far Plane", &App->editorCamera->farPlaneDistance)) {
					App->editorCamera->SetFarPlane(App->editorCamera->farPlaneDistance);
				}

				if (ImGui::InputFloat("Aspect Ratio", &App->editorCamera->aspectRatio)) {
					App->editorCamera->SetAspectRatio(App->editorCamera->aspectRatio);
				}

				ImGui::ColorEdit3("BG Color", App->renderer->bgColor.ptr());

			}
		}
	}
	if (ImGui::CollapsingHeader("About")) {
		static char engineNameBuff[32] = "Master Engine";
		static char descBuff[128] = "Engine made for the master's degree at UPC";
		static char authorBuff[32] = "By David Sierra";
		ImGui::InputText("Engine Name", engineNameBuff, IM_ARRAYSIZE(engineNameBuff));
		ImGui::InputText("Description", descBuff, IM_ARRAYSIZE(descBuff));
		ImGui::InputText("Author", authorBuff, IM_ARRAYSIZE(authorBuff));

		if (ImGui::CollapsingHeader("Libraries used")) {
			ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "Glew Version %s", glew_version);

			ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "MathGeoLib v1.5");

			ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "ImGui version %s", imgui_version);

			ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "SDL version %d.%d.%d", sdl_version.major, sdl_version.major, sdl_version.patch);
		}

		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1), "License: GNU General Public License v3.0");

	}

	ImGui::End();
}