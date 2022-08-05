#pragma once
#include "Components/Component.h"
#include "Components/Transform.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

class GameObject
{
public:
	Transform* transform;

	GameObject();
	~GameObject();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

private:
	std::vector<Component*> components;
};