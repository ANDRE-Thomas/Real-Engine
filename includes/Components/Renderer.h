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
	static void RenderAll(Camera* camera);
	static void UpdateGlobalBuffers();

private:
	static std::vector<Renderer*> renderers;
	static GLuint lightsSSBO;

public:
	Renderer(Model* model, Material* material);
	Renderer(Model* model);
	~Renderer();

	void Render(Camera* camera);

private:
	Model* model;
	Material* material;

	int nBuffer;
	std::vector<GLuint> VAO, VBO, EBO;

	void UpdateBuffers();
};