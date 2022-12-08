#include "Real Engine/Objects/GameObject.h"

#include "Real Engine/Components/Component.h"
#include "Real Engine/Components/Transform.h"

#include "Real Engine/Core/Log.h"

#pragma region Static

std::vector<GameObject*> GameObject::gameObjects;

void GameObject::UpdateAllGameObjects()
{
	for (size_t i = gameObjects.size(); i-- > 0;)
		gameObjects[i]->Update();
}

#pragma endregion

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);

	gameObjects.push_back(this);
}

GameObject::~GameObject()
{
	for (size_t i = components.size(); i-- > 0;)
		RemoveComponent(components[i]);

	auto result = std::find(gameObjects.begin(), gameObjects.end(), this);
	if (result != gameObjects.end())
		gameObjects.erase(result);
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}

void GameObject::AddComponent(Component* component)
{
	if (component->parent != nullptr)
		Log::Error("Component can't have more than one parent.");

	component->RegisterParent(this);
	components.push_back(component);
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

void GameObject::Update()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->Update();
}