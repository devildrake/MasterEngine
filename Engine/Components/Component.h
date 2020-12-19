#ifndef COMPONENT_H
#define COMPONENT_H
#include<string>
#include "../MathGeoLib/MathGeoLib.h"
class GameObject;

class Component {
public:
	GameObject* owner;
	std::string name;
	enum ComponentType {
		INVALID,
		CTTransformation,
		CTMeshRenderer,
		CTCamera,
		CTLight
	};
	ComponentType type;
	bool enabled;
public:
	Component(ComponentType aType, GameObject* anOwner);
	virtual ~Component();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
	virtual void DrawEditor();
	virtual void DrawGizmos();
	virtual void OnNewParent(GameObject* prevParent, GameObject* newParent);
	std::string Component::ComponentTypeToString(ComponentType type)const;
	virtual void OnTransformModified(float3 newPos, Quat newRot);
	virtual void OnSave();
	bool GetGlobalEnabled()const;
	//GameObject* owner;
};


#endif