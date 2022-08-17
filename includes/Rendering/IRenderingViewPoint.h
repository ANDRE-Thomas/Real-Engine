#pragma once

#include <stdexcept>

#include "Components/Component.h"
#include "Components/Transform.h"

class IRenderingViewPoint : public Component
{
public:
	mat4 GetViewProjectionMatrix() { return projectionMatrix * inverse(GetTransform()->GetMatrix4x4()); };

protected:
	mat4 projectionMatrix = mat4(1);

	IRenderingViewPoint(mat4 projectionMatrix) : Component()
	{
		this->projectionMatrix = projectionMatrix;
	}
};