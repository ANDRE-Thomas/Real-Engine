#include "Program.h"

#include "Objects/GameObject.h"
#include "Rendering/Mesh.h"
#include "Components/Renderer.h"
#include "Display/WindowOptions.h"
#include "Log.h"

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
	
	GameObject* cube = new GameObject();

	cube->AddComponent(new Renderer(Model::LoadModel("res/models/Dino.stl"), Material::GetMaterial("res/shaders/", "default")));
	cube->transform->SetPosition(vec3(-3, 0, 0));
	
	camera->transform->SetPosition(vec3(2, 0, 10));
	camera->transform->RotateAxis(vec3(0, 1, 0), 45.0f);
}

void Program::StartLoop()
{
	do
	{
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