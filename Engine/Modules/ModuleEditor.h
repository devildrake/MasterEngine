#ifndef MODULE_EDITOR_H
#define MODULE_EDITOR_H
#include "Module.h"

#include <vector>
#include <string>
#include <PreciseTimer.h>
#include "../MathGeoLib/Math/float3.h"
class ConfigWindow;
class ConsoleWindow;
class AboutWindow;
class EditorMainMenu;

class ModuleWindow;
class ModuleRender;
class ModuleEditorCamera;
class PropertiesWindow;
class SceneWindow;
class GameObjectHierarchyWindow;
class GameObject;
class ImGuiWindow;

#define FRAMECOUNT 60
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class ModuleEditor :public Module {
public:
	EditorMainMenu* mainMenu;

private:
	ConfigWindow* configWindow;
	ConsoleWindow* console;
	AboutWindow* aboutWindow;
	PropertiesWindow* propertiesWindow;
	SceneWindow* sceneWindow;
	GameObjectHierarchyWindow* hierarchyWindow;
	float gridMinSquares, gridMaxSquares, gridPosY, gridStep;
	float3 gridColor;
	GameObject* currentTarget;
	std::vector<ImGuiWindow*>windows;
	int frameCap;
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update()  override;
	UpdateStatus PostUpdate() override;
	bool CleanUp()  override;
	friend class ConfigWindow;
	ConsoleWindow* GetConsole()const;
	PropertiesWindow* GetProperties()const;
	SceneWindow* GetScene()const;
#ifdef _WIN32
#include <shellapi.h>
	static void open_url(const std::string& url) {
		ShellExecute(GetActiveWindow(),
			"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#endif
	void SetTargetObject(GameObject* newTarget);
	GameObject* GetTargetObject()const;

	void DrawMenu();
	void DrawGizmos();

};

#endif