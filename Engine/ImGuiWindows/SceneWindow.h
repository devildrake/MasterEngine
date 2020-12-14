#ifndef SCENE_WINDOW_H
#define SCENE_WNIDOW_H
#include "ImGuiWindow.h"
#include <vector>


class SceneWindow :public ImGuiWindow {
private:
	ImVec2 prevSize;
	bool isHovered;
public:
	SceneWindow(const char* windowName, int id);
	~SceneWindow();
	void Draw()override;
	bool IsMouseOverWindow()const;
	const bool& IsHovered()const;
	ImVec2 GetSize()const;
};


#endif // !SCENE_WINDOW_H
