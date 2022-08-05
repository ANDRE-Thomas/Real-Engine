#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 MVP;

out vec3 fragmentColor;

void main(){
  gl_Position = MVP * vec4(position, 1);

  fragmentColor = vec3(1,1,1);;
}