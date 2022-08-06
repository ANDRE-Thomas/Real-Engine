#pragma once

#include "Components/Component.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Objects/Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer : public Component
{
public:
	Renderer(Mesh* mesh, Material* material);
	~Renderer();

	static void RenderAll(Camera* camera);

	void Render(Camera* camera);

private:
	static std::vector<Renderer*> renderers;

	Mesh* mesh;
	Material* material;

	GLuint VBO;

	void UpdateBuffers();
};