#version 460 core

in VsOut{
	vec3 fragPosition;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
	mat3 TBN;
} vsOut;

struct Material{
	sampler2D diffuse;
	sampler2D ambient;
	sampler2D normal;
	sampler2D specular;
	sampler2D roughness;
};

struct Light{
	vec3 lightPosition;
	vec3 lightColor;
};

uniform vec3 viewPosition;
uniform Material material;
uniform Light mainLight;

out vec4 fragColor;

void main(){
    vec3 ambient = vec3(0.1) * mainLight.lightColor * texture(material.diffuse, vsOut.texCoord).rgb;

    vec3 norm = texture(material.normal, vsOut.texCoord).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(vsOut.TBN * norm); 

    vec3 lightDir = normalize(mainLight.lightPosition - vsOut.fragPosition);
    vec3 viewDir = normalize(viewPosition - vsOut.fragPosition);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0f);
    vec3 specular = mainLight.lightColor * (spec * texture(material.specular, vsOut.texCoord).rgb);  

    // Diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mainLight.lightColor * (diff * texture(material.diffuse, vsOut.texCoord).rgb);
        
    vec3 result = texture(material.ambient, vsOut.texCoord).r * (ambient + diffuse + specular);
    fragColor = vec4(result, 1);
}