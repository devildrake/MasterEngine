#ifndef MODULE_EDITOR_H
#define MODULE_EDITOR_H
#include "Module.h"

#include <vector>
#include <string>
#include "../Utilities/PreciseTimer.h"
#include "../MathGeoLib/Math/float3.h"
class ConfigWindow;
class ConsoleWindow;
class EditorMainMenu;

class ModuleWindow;
class ModuleRender;
class ModuleEditorCamera;
class PropertiesWindow;
#define FRAMECOUNT 60
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class ModuleEditor :public Module {
private:
	ConfigWindow* configWindow;
	ConsoleWindow* console;
	EditorMainMenu* mainMenu;
	PropertiesWindow* propertiesWindow;
	float gridMinSquares, gridMaxSquares, gridPosY, gridStep;
	float3 gridColor;

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

#ifdef _WIN32
#include <shellapi.h>
	static void open_url(const std::string& url)
	{
		ShellExecute(GetActiveWindow(),
			"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#endif

};

#endif