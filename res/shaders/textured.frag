#version 460 core
#extension GL_ARB_bindless_texture : require

in VsOut{
	vec3 fragPosition;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
	mat3 TBN;
} vsOut;

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

struct LightInfos{
	int type;
	vec4 lightPosition;
	vec4 lightColor;

	float constant;
	float linear;
	float quadratic;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	mat4 lightSpaceMatrix;
	uvec2 shadowMap;
};

layout (std140, binding = 0, column_major) readonly buffer lightsSSBO
{
	LightInfos lightInfos[];
};

struct Material{
	sampler2D diffuse;
	sampler2D ambient;
	sampler2D normal;
	sampler2D specular;
	sampler2D roughness;
};

uniform vec3 viewPosition;
uniform Material material;

out vec4 fragColor;

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	// Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow

	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);  

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main(){

	vec3 norm = texture(material.normal, vsOut.texCoord).rgb;
	norm = norm * 2.0 - 1.0;   
	norm = normalize(vsOut.TBN * norm); 

	vec3 viewDir = normalize(viewPosition - vsOut.fragPosition);

	vec3 result = vec3(0.0,0.0,0.0);

	for(int i = 0; i < lightInfos.length(); i++)
	{
		LightInfos light = lightInfos[i];

		float distanceFromLight = length(vec3(light.lightPosition) - vsOut.fragPosition);
		float attenuation = 1.0 / (light.constant + light.linear * distanceFromLight + light.quadratic * (distanceFromLight * distanceFromLight));    

		vec3 ambient = vec3(light.ambient) * vec3(light.lightColor) * texture(material.diffuse, vsOut.texCoord).rgb;

		vec3 lightDir;

		if(light.type == DIRECTIONAL_LIGHT)
			lightDir = normalize(vec3(-light.lightPosition));
		else
			lightDir = normalize(vec3(light.lightPosition) - vec3(vsOut.fragPosition));

		// Specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0f);
		vec3 specular = vec3(light.specular) * vec3(light.lightColor) * (spec * texture(material.specular, vsOut.texCoord).rgb);  

		// Diffuse 
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = vec3(light.diffuse) * vec3(light.lightColor) * (diff * texture(material.diffuse, vsOut.texCoord).rgb);
        
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		float shadow = ShadowCalculation(sampler2D(light.shadowMap), light.lightSpaceMatrix * vec4(vsOut.fragPosition, 1), norm, lightDir);       
		vec3 lighting = ambient + (1.0f - shadow) * (diffuse + specular) * texture(material.ambient, vsOut.texCoord).r; 

		result += lighting;
	}

	fragColor = vec4(result, 1);
}