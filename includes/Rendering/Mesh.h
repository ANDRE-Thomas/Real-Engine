#pragma once
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

class Mesh
{
public:
	Mesh(const char* path);
	~Mesh();

	std::vector<Vertex> vertices;
};
