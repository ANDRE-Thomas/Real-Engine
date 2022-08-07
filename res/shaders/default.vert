#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormals;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 MVP;

out vec3 normals;
out vec2 texCoords;

void main(){
  gl_Position = MVP * vec4(inPosition, 1);

  normals = inNormals;
  texCoords = inTexCoords;
}