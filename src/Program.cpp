#include "Program.h"

#include "Objects/GameObject.h"
#include "Rendering/Mesh.h"
#include "Components/Renderer.h"
#include "Components/DirectionalLight.h"
#include "Display/WindowOptions.h"
#include "Log.h"

GameObject* backpack;

void Program::InitGraph()
{
	if (!glfwInit())
		throw GraphInitException("GLFW Init Error");

	try
	{
		WindowOptions options = WindowOptions("Real Engine", false, false, 1280, 720, 4, vec3(0, 0, 0));
		window = new Window(options);
	}
	catch (const GraphInitException& exception)
	{
		Stop();
		throw GraphInitException(exception);
	}

	camera = new Camera(60.0f, window->GetAspectRatio(), 0.1f, 100.0f);

	GameObject* mainlight = new GameObject();
	mainlight->AddComponent(new DirectionalLight(vec3(1.0f, 1.0f, 1.0f)));
	mainlight->transform->SetPosition(vec3(3, 3, 5));

	backpack = new GameObject();

	Material* backpackMaterial = Material::GetMaterial("res/shaders/", "default");
	backpackMaterial->AddTextures(std::vector<Texture> {
		Texture::LoadTexture("res/models/backpack/ambient.jpg", "ambient"),
		Texture::LoadTexture("res/models/backpack/diffuse.jpg", "diffuse"),
		Texture::LoadTexture("res/models/backpack/normal.png", "normal"),
		Texture::LoadTexture("res/models/backpack/roughness.jpg", "roughness"),	
		Texture::LoadTexture("res/models/backpack/specular.jpg", "specular")
	});

	backpack->AddComponent(new Renderer(Model::LoadModel("res/models/backpack/backpack.obj", false), backpackMaterial));
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