#pragma once
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

class Mesh
{
public:
	Mesh(const char* path);
	~Mesh();

	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
};
