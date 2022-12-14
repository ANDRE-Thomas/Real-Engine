#include "Real Engine/Core/Program.h"

#include "Real Engine/Core/Inputs.h"
#include "Real Engine/Core/Time.h"

#include "Real Engine/Objects/GameObject.h"
#include "Real Engine/Rendering/Mesh.h"
#include "Real Engine/Components/Renderer.h"
#include "Real Engine/Components/Transform.h"
#include "Real Engine/Components/DirectionalLight.h"
#include "Real Engine/Rendering/Display/Window.h"
#include "Real Engine/Rendering/DefaultRP.h"
#include "Real Engine/Components/ViewController.h"
#include "Real Engine/Core/Log.h"

GameObject* backpack;

Window* Program::window;

void Program::Init()
{
	if (!glfwInit())
		throw GraphInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Real Engine", false, true, 1280, 720, new DefaultRP(), 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const GraphInitException& exception)
	{
		Stop();
		throw GraphInitException(exception);
	}

	Inputs::InitInputs(window->GetGLFWWindow());

	//All of the following code is temporary while scene loading isn't done
	GameObject* camera = new GameObject();
	camera->AddComponent(new Camera(60.0f, 0.1f, 100.0f));
	camera->transform->position = vec3(6, 0, 0);
	camera->transform->RotateAxis(vec3(0, 1, 0), 45);

	camera->AddComponent(new ViewController());

	GameObject* mainlight = new GameObject();
	mainlight->AddComponent(new DirectionalLight(vec3(1.0f, 0.25f, 0.5f) / 2.0f));
	mainlight->transform->RotateAxis(vec3(0, 1, 0), -5);
	mainlight->transform->position = vec3(0, 0, 5);

	mainlight = new GameObject();
	mainlight->AddComponent(new DirectionalLight(vec3(0.75f, 0.5f, 1.0f) / 2.0f));
	mainlight->transform->RotateAxis(vec3(0, 1, 0), 5);
	mainlight->transform->position = vec3(0, 0, 5);

	backpack = new GameObject();

	Material* backpackMaterial = Material::GetMaterial("res/shaders/", "textured");
	backpackMaterial->AddTextures(std::vector<Texture> {
		Texture::LoadTexture("res/models/backpack/ambient.jpg", "ambient"),
			Texture::LoadTexture("res/models/backpack/diffuse.jpg", "diffuse"),
			Texture::LoadTexture("res/models/backpack/normal.png", "normal"),
			Texture::LoadTexture("res/models/backpack/roughness.jpg", "roughness"),
			Texture::LoadTexture("res/models/backpack/specular.jpg", "specular")
	});

	backpack->AddComponent(new Renderer(Model::LoadModel("res/models/cube.obj", false)));
	backpack->transform->position = vec3(0, 0, -15);
	backpack->transform->scale = vec3(5, 5, 1);

	backpack = new GameObject();
	backpack->AddComponent(new Renderer(Model::LoadModel("res/models/backpack/backpack.obj", false), backpackMaterial));
	backpack->transform->position = vec3(0, 0, -6);

	GameObject* ground = new GameObject();

	ground->AddComponent(new Renderer(Model::LoadModel("res/models/cube.obj", false)));
	ground->transform->position = vec3(0, -4, -6);
	ground->transform->scale = vec3(100, 1, 100);
}

void Program::StartLoop()
{
	do
	{
		backpack->transform->RotateAxis(vec3(0, 1, 0), static_cast<float>(Time::GetDeltaTime()) * 90.0f);

		Time::Update();
		Inputs::Update();

		GameObject::UpdateAllGameObjects();
		window->Draw(Camera::GetMainCamera());
	} while (!window->CloseRequested());

	Stop();
}

void Program::Stop()
{
	delete(window);

	glfwTerminate();
}