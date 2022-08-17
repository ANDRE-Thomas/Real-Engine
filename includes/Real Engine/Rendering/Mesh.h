#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include <vector>
#include <string>

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices);
	~Mesh();
};
