#include "Rendering/Mesh.h"

#include <glm/gtx/string_cast.hpp>
#include "Log.h"
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
}

Mesh::~Mesh()
{
}