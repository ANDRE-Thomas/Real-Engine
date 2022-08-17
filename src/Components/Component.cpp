#include "Components/Component.h"

#include <stdexcept>

#include "Objects/GameObject.h"
#include "Log.h"

Component::Component() 
{
}

Component::~Component()
{
}

GameObject* Component::GetParent()
{
	if (parent == nullptr)
		throw std::runtime_error("Component has no parent.");

	return parent;
}

Transform* Component::GetTransform()
{
	if (parent == nullptr)
		throw std::runtime_error("Component has no parent.");

	return parent->transform;
}

void Component::RegisterParent(GameObject* parent)
{
	if (this->parent != nullptr)
	{
		Log::Warning("Component can't have more than one parent.");
		return;
	}

	this->parent = parent;
}