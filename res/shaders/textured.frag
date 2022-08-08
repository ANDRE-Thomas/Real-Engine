#version 460 core

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
};

layout (shared, binding = 0) readonly buffer lightsSSBO
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

void main(){
    vec3 ambient = vec3(0.1) * vec3(lightInfos[0].lightColor) * texture(material.diffuse, vsOut.texCoord).rgb;

    vec3 norm = texture(material.normal, vsOut.texCoord).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(vsOut.TBN * norm); 

    vec3 lightDir = normalize(vec3(-lightInfos[0].lightPosition));
    vec3 viewDir = normalize(viewPosition - vsOut.fragPosition);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0f);
    vec3 specular = vec3(lightInfos[0].lightColor) * (spec * texture(material.specular, vsOut.texCoord).rgb);  

    // Diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(lightInfos[0].lightColor) * (diff * texture(material.diffuse, vsOut.texCoord).rgb);
        
    vec3 result = texture(material.ambient, vsOut.texCoord).r * (ambient + diffuse + specular);
    fragColor = vec4(result, 1);
}