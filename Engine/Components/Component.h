#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
public:
	GameObject* owner;
	enum ComponentType {
		Transformation,
		Mesh,
		Material,
		Light
	};
	ComponentType type;
	bool active;
public:
	Component();
	~Component();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	//GameObject* owner;
};


#endif