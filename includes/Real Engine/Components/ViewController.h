#pragma once

#include "Real Engine/Components/Component.h"

class ViewController : public Component
{
public:
	ViewController();
	~ViewController();

private:
	float mouseSpeed = 35;
	float movementSpeed = 10;
	vec2 previousMousePosition;

	virtual void Update();
};