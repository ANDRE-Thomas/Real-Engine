#include "Rendering/Mesh.h"

#include <glm/gtx/string_cast.hpp>
#include "Log.h"
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
}