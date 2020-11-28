#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include "ImGuiWindow.h"
#include <SDL.h>


class AboutWindow :public ImGuiWindow {
private:
	SDL_version sdl_version;
	const char* imgui_version;
	const char* glew_version;
public:
	AboutWindow(const char* windowName);
	~AboutWindow();
	void Draw();
public:
	friend class ModuleEditor;
	friend class EditorMainMenu;

};

//class AboutWindow {// :public ImGuiWindow {
//	AboutWindow();
//	~AboutWindow();


	//private:
	//	//SDL_version sdl_version;
	//	const char* imgui_version;
	//	const char* glew_version;
	//public:
	//	AboutWindow(const char* windowName);
	//	~AboutWindow();
	//	void Draw();
	//public:
	//	friend class ModuleEditor;
	//	friend class EditorMainMenu;
	//}
//}
#endif