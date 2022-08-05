#pragma once

class GameObject; //No include because of circular dependency

class Component
{
public:
	Component();
	~Component();

	GameObject* GetParent();
	void Register(GameObject* parent);

private:
	GameObject* gameObject;
};