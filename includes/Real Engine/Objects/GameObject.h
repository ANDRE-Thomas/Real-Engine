#pragma once

#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include <vector>

#include "Real Engine/Core/Program.h"

class Transform;
class Component;

class GameObject
{
	friend class Program;

public:
	template<class T>
	static T* FindComponentOfType()
	{
		for (size_t i = gameObjects.size(); i-- > 0;)
		{
			std::vector<Component*> components = gameObjects[i]->components;
			for (size_t j = components.size(); j-- > 0;)
			{
				if (dynamic_cast<T*>(components[j]) != nullptr)
					return dynamic_cast<T*>(components[j]);
			}
		}

		return nullptr;
	}

	template<class T>
	static std::vector<T*> FindAllComponentsOfType()
	{
		std::vector<T*> foundComponents;
		for (size_t i = gameObjects.size(); i-- > 0;)
		{
			std::vector<Component*> components = gameObjects[i]->components;
			for (size_t j = components.size(); j-- > 0;)
			{
				if (dynamic_cast<T*>(components[j]) != nullptr)
					foundComponents.push_back(dynamic_cast<T*>(components[j]));
			}
		}

		return foundComponents;
	}

private:
	static std::vector<GameObject*> gameObjects;

	static void UpdateAllGameObjects();

public:
	Transform* transform;

	GameObject();

	//Must be implemented in .h to recognize the class used in template
	template<class T>
	T* GetComponent()
	{
		for (size_t i = components.size(); i-- > 0;)
		{
			if (dynamic_cast<T*>(components[i]) != nullptr)
				return dynamic_cast<T*>(components[i]);
		}

		return nullptr;
	}

	//Must be implemented in .h to recognize the class used in template
	template<class T>
	bool TryGetComponent(T& component)
	{
		for (size_t i = components.size(); i-- > 0;)
		{
			if (dynamic_cast<T*>(components[i]) != nullptr)
			{
				component = dynamic_cast<T*>(&components[i]);
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

	void Update();
};