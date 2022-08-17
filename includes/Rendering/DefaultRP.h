#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Rendering/RenderingPipeline.h"
#include "Rendering/Material.h"
#include "Components/Renderer.h"

class DefaultRP : public RenderingPipeline
{
public:
	DefaultRP();
	~DefaultRP();

	virtual void Init();
	virtual void Render(IRenderingViewPoint* viewPoint);

	void SetFirstPassParameters(Material* material, IRenderingViewPoint* viewpoint, Renderer* renderer);
	void RenderFirstPass(IRenderingViewPoint* viewPoint);

	void SetSecondPassParameters(Material* material, IRenderingViewPoint* viewpoint, Renderer* renderer);
	void RenderSecondPass(IRenderingViewPoint* viewPoint);

private:
	GLuint lightsSSBO, shadowsFBO;
};