#include "Objects/GameObject.h"

#include "Components/Component.h"
#include "Components/Transform.h"

#include "Log.h"

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);
}

GameObject::~GameObject()
{
	for (size_t i = components.size(); i-- > 0;)
		RemoveComponent(components[i]);
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}


void GameObject::AddComponent(Component* component)
{
	if (component->GetParent() != nullptr)
		Log::Error("Component can't have more than one parent.");

	components.push_back(component);
	component->RegisterParent(this);
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