#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include <vector>

class Transform;
class Component;

class GameObject
{
public:
	Transform* transform;

	GameObject();

	template<class T>
	T* GetComponent()
	{
		for (size_t i = components.size(); i-- > 0;)
		{
			if (reinterpret_cast<T*>(components[i]) != nullptr)
				return reinterpret_cast<T*>(components[i]);
		}

		return nullptr;
	}

	template<class T>
	bool TryGetComponent(T& component)
	{
		for (size_t i = components.size(); i-- > 0;)
		{
			if (reinterpret_cast<T*>(components[i]) != nullptr)
			{
				component = reinterpret_cast<T>(&components[i]);
				return true;
			}
		}

		return false;
	}

	std::vector<Component*> GetComponents();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

protected:
	~GameObject();

private:
	std::vector<Component*> components;
};