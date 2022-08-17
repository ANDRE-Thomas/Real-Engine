#include "Real Engine/Rendering/DefaultRP.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <functional>

#include "Real Engine/Objects/GameObject.h"
#include "Real Engine/Components/Renderer.h"
#include "Real Engine/Components/Light.h"

const int SHADOW_MAPS_WIDTH = 1024;
const int SHADOW_MAPS_HEIGHT = 1024;

DefaultRP::DefaultRP() : RenderingPipeline()
{
	lightsSSBO = NULL;
	shadowsFBO = NULL;

	Light::SetDepthMapsSize(SHADOW_MAPS_WIDTH, SHADOW_MAPS_HEIGHT);
}

DefaultRP::~DefaultRP()
{
	if (lightsSSBO != NULL)
		glDeleteBuffers(1, &lightsSSBO);

	if (shadowsFBO != NULL)
		glDeleteFramebuffers(1, &shadowsFBO);
}

void DefaultRP::Init()
{
	glGenBuffers(1, &lightsSSBO);

	glGenFramebuffers(1, &shadowsFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowsFBO);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefaultRP::Render(IRenderingViewPoint* viewPoint)
{
	std::vector<LightInfos> lightInfos = Light::GetAllLightsInfos();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightInfos.size() * sizeof(LightInfos), &lightInfos[0], GL_STATIC_READ);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	RenderFirstPass(viewPoint); //Depth only for lights
	RenderSecondPass(viewPoint); //Lights calculations
}

#pragma region First Pass

void DefaultRP::SetFirstPassParameters(Material* material, IRenderingViewPoint* viewpoint, Renderer* renderer)
{
	material->SetMat4x4("modelMat", renderer->GetParent()->transform->GetMatrix4x4());
}

void DefaultRP::RenderFirstPass(IRenderingViewPoint* viewPoint)
{
	std::vector<Light*> lights = Light::GetAllLights();

	Material* simpleDepth = Material::GetMaterial("res/internals/shaders/", "simpleDepthShader");
	glUseProgram(simpleDepth->GetProgramID());

	std::vector<GLint> viewportSize(4);
	glGetIntegerv(GL_VIEWPORT, &viewportSize[0]);

	glViewport(0, 0, SHADOW_MAPS_WIDTH, SHADOW_MAPS_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowsFBO);
	glCullFace(GL_FRONT);

	for (size_t i = 0; i < lights.size(); i++)
	{
		simpleDepth->SetMat4x4("lightSpaceMat", lights[i]->GetLightInfos().lightSpaceMatrix);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lights[i]->GetDepthMapID(), 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		IRenderingViewPoint* renderingPoint = dynamic_cast<IRenderingViewPoint*>(lights[i]);

		Renderer::RenderAll(renderingPoint, std::bind(&DefaultRP::SetFirstPassParameters, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), simpleDepth);
	}

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewportSize[0], viewportSize[1], viewportSize[2], viewportSize[3]);
}

#pragma endregion

#pragma region Second Pass

void DefaultRP::SetSecondPassParameters(Material* material, IRenderingViewPoint* viewpoint, Renderer* renderer)
{
	if (material->HasParameter("viewPosition"))
		material->SetVec3("viewPosition", viewpoint->GetTransform()->position);

	if (material->HasParameter("MVP"))
		material->SetMat4x4("MVP", viewpoint->GetViewProjectionMatrix() * renderer->GetParent()->transform->GetMatrix4x4());

	if (material->HasParameter("modelMat"))
		material->SetMat4x4("modelMat", renderer->GetParent()->transform->GetMatrix4x4());

	if (material->HasParameter("normalMat"))
		material->SetMat3x3("normalMat", transpose(inverse(mat3x3(renderer->GetParent()->transform->GetMatrix4x4()))));

	if (material->HasParameter("material.diffuse"))
	{
		std::vector<Texture> textures = material->GetTextures();
		for (size_t j = 0; j < textures.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);

			material->SetInt("material." + textures[j].GetName(), j); //Bind uniform to texture location
			glBindTexture(GL_TEXTURE_2D, textures[j].GetID()); //Bind texture to texture location
		}

		glActiveTexture(GL_TEXTURE0);
	}
}

void DefaultRP::RenderSecondPass(IRenderingViewPoint* viewPoint)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightsSSBO);

	Renderer::RenderAll(viewPoint, std::bind(&DefaultRP::SetSecondPassParameters, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

#pragma endregion