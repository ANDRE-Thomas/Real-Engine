#pragma once

#include "Objects/Object.h"
#include "Components/Transform.h"

class GameObject : public Object
{
public:
	Transform* transform;

	GameObject();
	~GameObject() {}
};