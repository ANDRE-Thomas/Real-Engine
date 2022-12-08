#include "Real Engine/Components/ViewController.h"

#include "Real Engine/Core/Inputs.h"
#include "Real Engine/Core/Time.h"
#include "Real Engine/Components/Transform.h"

ViewController::ViewController() : Component()
{
	previousMousePosition = Inputs::GetMousePosition();
}

ViewController::~ViewController()
{
}

void ViewController::Update()
{
	vec2 mousePosition = Inputs::GetMousePosition();

	Transform* transform = GetTransform();
	float deltaTime = static_cast<float>(Time::GetDeltaTime());

	transform->RotateAxis(vec3(0, -1, 0), mouseSpeed * (mousePosition - previousMousePosition).x * deltaTime);
	transform->RotateAxis(transform->Right(), mouseSpeed * -(mousePosition - previousMousePosition).y * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_W))
		transform->Translate(transform->Forward() * movementSpeed * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_S))
		transform->Translate(-transform->Forward() * movementSpeed * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_D))
		transform->Translate(transform->Right() * movementSpeed * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_A))
		transform->Translate(-transform->Right() * movementSpeed * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_E))
		transform->Translate(transform->Up() * movementSpeed * deltaTime);

	if (Inputs::GetKeyHold(GLFW_KEY_Q))
		transform->Translate(-transform->Up() * movementSpeed * deltaTime);

	previousMousePosition = mousePosition;
}