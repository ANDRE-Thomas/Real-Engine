#include "Objects/GameObject.h"

#include <stdexcept>
#include <algorithm>

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);
}

GameObject::~GameObject()
{
	for (size_t i = components.size(); i-- > 0;)
		delete(components[i]);
}

void GameObject::AddComponent(Component* component)
{
	if (component->GetParent() != nullptr)
		throw std::runtime_error("GameObjects can't share components!");

	components.push_back(component);
	component->Register(this);
}

void GameObject::RemoveComponent(Component* component)
{
	auto result = std::find(components.begin(), components.end(), component);

	if (result != components.end())
	{
		delete(components[result - components.begin()]);
		components.erase(result);
	}
}