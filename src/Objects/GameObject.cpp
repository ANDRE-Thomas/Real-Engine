#include "Objects/GameObject.h"

#include <stdexcept>
#include <algorithm>

GameObject::GameObject() : Object()
{
	transform = new Transform();
	AddChild(transform);
}