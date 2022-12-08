#include "Real Engine/Components/Component.h"

#include <stdexcept>

#include "Real Engine/Objects/GameObject.h"
#include "Real Engine/Core/Log.h"

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