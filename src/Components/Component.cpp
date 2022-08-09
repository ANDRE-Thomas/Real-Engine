#include "Components/Component.h"

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
	return parent;
}

Transform* Component::GetTransform()
{
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