#include "Program.h"

#include "Objects/GameObject.h"
#include "Rendering/Mesh.h"
#include "Components/Renderer.h"
#include "Display/WindowOptions.h"
#include "Log.h"

GameObject* backpack;

void Program::InitGraph()
{
	if (!glfwInit())
		throw GraphInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Game", false, false, 1280, 720, 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const GraphInitException& exception)
	{
		Stop();
		throw GraphInitException(exception);
	}

	camera = new Camera(60.0f, window->GetAspectRatio(), 0.1f, 100.0f);

	backpack = new GameObject();

	backpack->AddComponent(new Renderer(Model::LoadModel("res/models/backpack/backpack.obj"), Material::GetMaterial("res/shaders/", "default")));
	backpack->transform->SetPosition(vec3(0, -1, -6));
}

void Program::StartLoop()
{
	do
	{
		backpack->transform->RotateAxis(vec3(0, 1, 0), window->DeltaTime() * 90.0f);
		window->Draw(camera);
	} while (!window->CloseRequested());

	Stop();
}

void Program::Stop()
{
	if (window != nullptr)
		delete(window);

	glfwTerminate();
}