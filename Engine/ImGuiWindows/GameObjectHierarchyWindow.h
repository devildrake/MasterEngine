#ifndef GAMEOBJECT_HIERARCHY_WINDOW_H
#define GAMEOBJECT_HIERARCHY_WINDOW_H

#include "ImGuiWindow.h"
#include "../Application.h"

class ModuleScene;
class GameObject;

class GameObjectHierarchyWindow :public ImGuiWindow {
private:
	ModuleScene* currentScene;
	std::pair<int, int> DrawChildren(GameObject* targets)const;
	int prevNodeClicked;
public:

	GameObjectHierarchyWindow(const char* windowName, ModuleScene* currentScene);
	~GameObjectHierarchyWindow();
	void Draw();
	const bool IsMouseOverWindow()const;
	friend class ModuleEditor;
};

#endif