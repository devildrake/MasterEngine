#ifndef MODULE_EDITOR_H
#define MODULE_EDITOR_H
#include "Module.h"
#include "../Utilities/Globals.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"
#include "../ConsoleWindow.h"
#include "../ConfigWindow.h"
#include <vector>
#include <string>

#define FRAMECOUNT 30
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class ModuleEditor :public Module {
private:
	ConfigWindow* configMenu;
	ConsoleWindow* console;
	int frameCap;
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start()override;
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	friend class ConfigWindow;
	ConsoleWindow* GetConsole();
};

#endif