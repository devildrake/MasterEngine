#ifndef GAMEOBJECT_HIERARCHY_WINDOW_H
#define GAMEOBJECT_HIERARCHY_WINDOW_H

#include "ImGuiWindow.h"
#include "../Application.h"

class ModuleScene;
class GameObject;

class GameObjectHierarchyWindow :public ImGuiWindow {
private:
	ModuleScene* currentScene;
	void DrawChildren(GameObject* targets, bool drawSelf = true)const;
	GameObject* prevSelectedNode;
public:

	GameObjectHierarchyWindow(const char* windowName, int id, ModuleScene* currentScene);
	~GameObjectHierarchyWindow();
	void Draw()override;
	const bool IsMouseOverWindow()const;
	friend class ModuleEditor;
};

#endif