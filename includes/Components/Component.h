#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
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
};