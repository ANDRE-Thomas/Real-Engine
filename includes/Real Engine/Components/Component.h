#pragma once

#include <glm/gtx/quaternion.hpp>
using namespace glm;

class Transform;
class GameObject;

class Component
{
	friend class GameObject;

public:
	Component();

	GameObject* GetParent();
	Transform* GetTransform();

protected:
	virtual ~Component();

private:
	GameObject* parent = nullptr;

	void RegisterParent(GameObject* parent);

	virtual void Update() {};
};