#include "Objects/Object.h"

#include "Log.h"

Object::Object()
{
	parent = nullptr;
}

Object::~Object()
{
	for (size_t i = children.size(); i-- > 0;)
		RemoveChild(children[i]);
}

std::vector<Object*> Object::GetChildren()
{
	std::vector<Object*> result;

	for (size_t i = 0; i < children.size(); i++)
		if (dynamic_cast<Object*>(children[i]) != nullptr)
			result.push_back(children[i]);

	return result;
}

void Object::AddChild(Object* child)
{
	if (child->parent != nullptr)
		Log::Error("Component can't have more than one parent.");

	children.push_back(child);
	child->parent = this;
}

void Object::RemoveChild(Object* child)
{
	auto result = std::find(children.begin(), children.end(), child);

	if (result != children.end())
	{
		delete(children[result - children.begin()]);
		children.erase(result);
	}
}

Object* Object::GetParent()
{
	return parent;
}
