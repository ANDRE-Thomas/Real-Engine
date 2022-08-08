#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include <vector>

class Object
{
public:
	Object();
	virtual ~Object();

	Object* GetParent();

	template<class T>
	T* GetChild()
	{
		for (size_t i = 0; i < children.size(); i++)
			if (dynamic_cast<T*>(children[i]) != nullptr)
				return dynamic_cast<T*>(children[i]);

		return nullptr;
	}

	std::vector<Object*> GetChildren();

	void AddChild(Object* child);

	void RemoveChild(Object* child);

private:
	Object* parent;

	std::vector<Object*> children;
};
