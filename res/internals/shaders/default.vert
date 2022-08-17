#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 MVP;
uniform mat4 modelMat;
uniform mat3 normalMat;

out VsOut{
	vec3 fragPosition;
	vec3 normal;
} vsOut;

void main(){
	vec4 position = MVP * vec4(inPosition, 1);
	gl_Position = position;

	vsOut.fragPosition = vec3(modelMat * vec4(inPosition, 1));
	vsOut.normal = normalMat * inNormal;
}