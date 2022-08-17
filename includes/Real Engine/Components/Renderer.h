#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "Real Engine/Components/Component.h"
#include "Real Engine/Rendering/Model.h"
#include "Real Engine/Rendering/Material.h"
#include "Real Engine/Rendering/IRenderingViewPoint.h"

class Renderer : public Component
{
public:
	static void RenderAll(IRenderingViewPoint* viewPoint);
	static void RenderAll(IRenderingViewPoint* viewPoint, Material* material);
	static void RenderAll(IRenderingViewPoint* viewPoint, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters);
	static void RenderAll(IRenderingViewPoint* viewPoint, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters, Material* material);

private:
	static std::vector<Renderer*> renderers;

public:
	Renderer(Model* model);
	Renderer(Model* model, Material* material);

	void Render(IRenderingViewPoint* viewPoint, Material* material, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters = nullptr);

private:
	Model* model;
	Material* material;

	int nBuffer;
	std::vector<GLuint> VAO, VBO, EBO;

	~Renderer();

	void UpdateGeometryBuffers();
};