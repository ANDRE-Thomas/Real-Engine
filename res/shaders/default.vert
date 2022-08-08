#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

uniform mat4 MVP;
uniform mat3 normalMat;

out VsOut{
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
} vsOut;

void main(){
	vec4 position = MVP * vec4(inPosition, 1);
	gl_Position = position;

	vsOut.fragPosition = inPosition;
	vsOut.normal = normalMat * inNormal;
	vsOut.texCoord = inTexCoord;
}