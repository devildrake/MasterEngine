#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"
class Mesh;
class Material;

class ComponentMeshRenderer : public Component {

public:
	ComponentMeshRenderer(GameObject* anOwner);
	~ComponentMeshRenderer();
	void Enable()override;
	void Update()override;
	void Disable() override;

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* mat);

private:
	Mesh* mesh;
	Material* material;
};

#endif MESH_RENDERER_H