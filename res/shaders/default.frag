#version 460 core

in vec3 normals;
in vec2 texCoords;

out vec3 color;

uniform sampler2D specular;
uniform sampler2D diffuse;

void main(){
  color = texture(diffuse, texCoords).rgb;
}