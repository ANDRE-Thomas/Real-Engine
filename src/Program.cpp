#include "Program.h"

#include "Inputs.h"

#include "Objects/GameObject.h"
#include "Rendering/Mesh.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Components/DirectionalLight.h"
#include "Components/PointLight.h"
#include "Display/WindowOptions.h"
#include "Rendering/DefaultRP.h"
#include "Log.h"

GameObject* backpack;

Window* Program::window;
GameObject* Program::camera;

void Program::Init()
{
	if (!glfwInit())
		throw GraphInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Real Engine", false, false, 1280, 720, new DefaultRP(), 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const GraphInitException& exception)
	{
		Stop();
		throw GraphInitException(exception);
	}

	Inputs::SetCursorVisibility(false);

	//All of the following code is temporary while scene loading isn't done
	camera = new GameObject();
	camera->AddComponent(new Camera(60.0f, window->GetAspectRatio(), 0.1f, 100.0f));
	camera->transform->position = vec3(6, 0, 0);
	camera->transform->RotateAxis(vec3(0, 1, 0), 45);

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
		backpack->transform->RotateAxis(vec3(0, 1, 0), window->DeltaTime() * 90.0f);
		window->Draw(camera->GetComponent<Camera>());
	} while (!window->CloseRequested());

	Stop();
}

void Program::Stop()
{
	if (window != nullptr)
		delete(window);

	glfwTerminate();
}