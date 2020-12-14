#ifndef PROPERTIES_WINDOW_H
#define PROPERTIES_WINDOW_H
#include "ImGuiWindow.h"
#include "../MathGeoLib/Math/float3.h"
#include <string>
/*. A properties window with three sections: transformation, geometry and texture. All should give
read-only information about the current loaded meshes and texture (triangle count, texture size).*/

class GameObject;

class PropertiesWindow :public ImGuiWindow {
private:

public:
	PropertiesWindow(const char* windowName, int id);
	~PropertiesWindow();
	void Draw()override;

	friend class ModuleEditor;
};


#endif
