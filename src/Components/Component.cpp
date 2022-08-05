#include "Components/Component.h"

Component::Component()
{

}

Component::~Component()
{
}

GameObject* Component::GetParent()
{
	return gameObject;
}

void Component::Register(GameObject* parent)
{
	gameObject = parent;
}