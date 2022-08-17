#include "Real Engine/Rendering/Mesh.h"

#include <vector>

#include "Real Engine/Log.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
}