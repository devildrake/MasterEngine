#ifndef SCENE_WINDOW_H
#define SCENE_WNIDOW_H
#include "ImGuiWindow.h"
#include <vector>


class SceneWindow :public ImGuiWindow {
private:
	std::pair<int, int> prevSize;
public:
	SceneWindow(const char* windowName);
	~SceneWindow();
	void Draw();
	bool IsMouseOverWindow()const;

};


#endif // !SCENE_WINDOW_H
