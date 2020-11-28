#ifndef PROPERTIES_WINDOW_H
#define PROPERTIES_WINDOW_H
#include "ImGuiWindow.h"
#include "../MathGeoLib/Math/float3.h"
#include <string>
/*. A properties window with three sections: transformation, geometry and texture. All should give
read-only information about the current loaded meshes and texture (triangle count, texture size).*/

class PropertiesWindow :public ImGuiWindow {
private:
	std::string modelName;
	int triangleCount;
	int textureSize;

public:
	PropertiesWindow(const char* windowName);
	~PropertiesWindow();
	void Draw();
	friend class ModuleEditor;
};


#endif
