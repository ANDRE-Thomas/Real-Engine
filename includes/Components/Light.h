#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <stdexcept>
#include <cstdlib>

#include "Rendering/IRenderingViewPoint.h"

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

struct LightInfos
{
	alignas(4) int type;
	alignas(16) vec4 lightPosition;
	alignas(16) vec4 lightColor;

	alignas(4) float constant;
	alignas(4) float linear;
	alignas(4) float quadratic;

	alignas(16) vec4 ambient;
	alignas(16) vec4 diffuse;
	alignas(16) vec4 specular;

	alignas(16) mat4 lightSpaceMatrix;
	alignas(8) GLuint64 shadowMap;

	LightInfos(int type, vec3 lightPosition, vec3 lightColor, float constant,
		float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular, mat4 lightSpaceMatrix, GLuint64 shadowMapHandle)
	{
		this->type = type;

		this->lightPosition = vec4(lightPosition, type == DIRECTIONAL_LIGHT ? 0 : 1);
		this->lightColor = vec4(lightColor, 1);

		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;

		this->ambient = vec4(ambient, 1);
		this->diffuse = vec4(diffuse, 1);
		this->specular = vec4(specular, 1);

		this->lightSpaceMatrix = lightSpaceMatrix;
		this->shadowMap = shadowMapHandle;
	}
};

class Light : public IRenderingViewPoint
{
public:
	static std::vector<Light*> GetAllLights();
	static std::vector<LightInfos> GetAllLightsInfos();

	static void SetDepthMapsSize(int width, int height);

private:
	static std::vector<Light*> lights;
	static int depthMapsWidth, depthMapsHeight;

public:
	vec3 lightColor;

	GLuint GetDepthMapID();
	GLuint64 GetDepthMapBindlessHandle();

	virtual LightInfos GetLightInfos() { throw std::runtime_error("GetLightInfos was not implemented"); };

protected:
	Light(mat4 projectionMatrix, vec3 lightColor);
	~Light();

private:
	GLuint depthMap = NULL;
	GLuint64 depthMapBindlessHandle = NULL;

	void GenerateDepthMap();
	void ReleaseDepthMap();
};