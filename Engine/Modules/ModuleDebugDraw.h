#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"
#include "../MathGeoLib/MathGeoLib.h"
#include "../MathGeoLib/Geometry/Frustum.h"
class DDRenderInterfaceCoreGL;

class ModuleDebugDraw : public Module {

public:

	ModuleDebugDraw();
	~ModuleDebugDraw();

	bool Init() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

	void DrawGrid(float gridMinSquares, float gridMaxSquares, float gridPos, float gridStep, float3 color);
	void DrawAxisTriad(float4x4 axisTransform = float4x4::identity, float axisWidth = 0.3f, float axisLength = 3.0f);
	void DrawFrustum(const Frustum& frustum);
	void DrawAABB(const AABB& aabb);
	void Draw(const float4x4& view, const float4x4& proj, unsigned width, unsigned height);
private:

	static DDRenderInterfaceCoreGL* implementation;
};

#endif /* _MODULE_DEBUGDRAW_H_ */
