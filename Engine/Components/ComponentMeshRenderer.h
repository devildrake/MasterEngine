#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"
#include "../MathGeoLib/MathGeoLib.h"
class Mesh;
class Material;

class ComponentMeshRenderer : public Component {

public:
	ComponentMeshRenderer(GameObject* anOwner);
	~ComponentMeshRenderer();
	void Enable()override;
	void Update()override;
	void Disable() override;
	void DrawEditor() override;
	void DrawGizmos() override;
	void OnTransformModified()override;
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* mat);
	void GenerateAABB();
	const AABB& GetAABB()const;
private:
	Mesh* mesh;
	Material* material;
	AABB localAxisAlignedBoundingBox;
	OBB localOrientedBoundingBox;

};

#endif MESH_RENDERER_H