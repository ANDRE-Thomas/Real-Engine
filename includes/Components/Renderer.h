#pragma once

#include "Components/Component.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Objects/Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer : public Component
{
public:
	Renderer(Model* model, Material* material);
	~Renderer();

	static void RenderAll(Camera* camera);

	void Render(Camera* camera);

private:
	static std::vector<Renderer*> renderers;

	Model* model;
	Material* material;

	int nBuffer;
	std::vector<GLuint> VAO, VBO, EBO;

	void UpdateBuffers();
};