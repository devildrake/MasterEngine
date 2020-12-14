#pragma once

#include <Globals.h>

class Application;

class Module {
private:
	const char* name;
public:

	Module(const char* aName = "") :name(aName) {

	}

	virtual ~Module() {

	}

	virtual bool Init() {
		return true;
	}

	virtual bool Start() {
		return true;
	}

	virtual UpdateStatus PreUpdate() {
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update() {
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate() {
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() {
		return true;
	}

	virtual void MainWindowResized(unsigned newW, unsigned newH) {

	}

	//virtual void SceneWindowResized(unsigned newW, unsigned newH) {

	//}

	const char* GetModuleName() {
		return name;
	}

};
