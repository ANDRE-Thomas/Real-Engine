#version 460 core

layout(location = 0) in vec3 inPosition;

uniform mat4 modelMat;
uniform mat4 lightSpaceMat;

void main()
{
    gl_Position = lightSpaceMat * modelMat * vec4(inPosition, 1.0);
}  