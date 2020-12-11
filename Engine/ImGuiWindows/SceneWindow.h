#ifndef SCENE_WINDOW_H
#define SCENE_WNIDOW_H
#include "ImGuiWindow.h"
#include <vector>


class SceneWindow :public ImGuiWindow {
private:
	std::pair<int, int> prevSize;
	bool mustFocus;
	bool isHovered;
public:
	SceneWindow(const char* windowName);
	~SceneWindow();
	void Draw()override;
	bool IsMouseOverWindow()const;
	const bool& IsHovered()const;
	void WindowFocused();
};


#endif // !SCENE_WINDOW_H
