#include "Components/Light.h"

#pragma region Static

std::vector<Light*> Light::lights;
int Light::depthMapsWidth, Light::depthMapsHeight;

std::vector<Light*> Light::GetAllLights()
{
	return lights;
}

std::vector<LightInfos> Light::GetAllLightsInfos()
{
	std::vector<LightInfos> lightInfos;
	for (size_t i = lights.size(); i-- > 0;)
		lightInfos.push_back(lights[i]->GetLightInfos());

	return lightInfos;
}

void Light::SetDepthMapsSize(int width, int height)
{
	depthMapsWidth = width;
	depthMapsHeight = height;

	for (int i = lights.size() - 1; i >= 0; i--)
		lights[i]->GenerateDepthMap();
}

#pragma endregion Static

Light::Light(mat4 shadowProjectionMatrix, vec3 lightColor) : IRenderingViewPoint(shadowProjectionMatrix)
{
	this->lightColor = lightColor;

	GenerateDepthMap();

	lights.push_back(this);
}

Light::~Light()
{
	ReleaseDepthMap();

	auto result = std::find(lights.begin(), lights.end(), this);
	if (result != lights.end())
		lights.erase(result);
}

GLuint Light::GetDepthMapID()
{
	return depthMap;
}

GLuint64 Light::GetDepthMapBindlessHandle()
{
	return depthMapBindlessHandle;
}

void Light::GenerateDepthMap()
{
	ReleaseDepthMap();

	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapsWidth, depthMapsHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D, 0);

	depthMapBindlessHandle = glGetTextureHandleARB(depthMap);
	glMakeTextureHandleResidentARB(depthMapBindlessHandle);
}

void Light::ReleaseDepthMap()
{
	if (depthMapBindlessHandle != NULL && glIsTextureHandleResidentARB(depthMapBindlessHandle))
		glMakeTextureHandleNonResidentARB(depthMapBindlessHandle);

	if (depthMap != NULL)
		glDeleteTextures(1, &depthMap);
}