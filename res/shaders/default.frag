#version 460 core

in VsOut{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
} vsOut;

struct LightInfos{
	vec4 lightPositionOrDirection;
	vec3 lightColor;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (binding = 0) readonly buffer lightsSSBO
{
	LightInfos lightInfos[];
};

uniform vec3 viewPosition;

out vec4 fragColor;

void main(){
    vec3 ambient = vec3(0.1) * lightInfos[0].lightColor * vec3(1,1,1);

    vec3 norm = normalize(vsOut.normal); 

    vec3 lightDir = normalize(vec3(lightInfos[0].lightPositionOrDirection) - vsOut.fragPosition);
    vec3 viewDir = normalize(viewPosition - vsOut.fragPosition);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0f);
    vec3 specular = lightInfos[0].lightColor * (spec * 0.5f);  

    // Diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightInfos[0].lightColor * (diff * 0.5f);
        
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1);
}