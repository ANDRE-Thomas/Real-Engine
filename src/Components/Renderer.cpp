#include "Components/Renderer.h"

std::vector<Renderer*> Renderer::renderers;

Renderer::Renderer(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;

	UpdateBuffers();

	renderers.push_back(this);
}

Renderer::~Renderer()
{
	if (mesh != nullptr)
		delete(mesh);

	auto result = std::find(renderers.begin(), renderers.end(), this);
	if (result != renderers.end())
		renderers.erase(result);
}

void Renderer::RenderAll(Camera* camera)
{
	for (int i = renderers.size() - 1; i >= 0; i--)
		renderers[i]->Render(camera);
}

void Renderer::Render(Camera* camera)
{
	glUseProgram(material->GetProgramID());

	// Send the transformation to the currently bound shader, in the "MVP" uniform
	material->SetMat4x4("MVP", camera->GetRenderMatrix() * GetParent()->transform->GetMatrix4x4());

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : uvs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)offsetof(Vertex, normal)	          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_TRUE,            // normalized?
		sizeof(Vertex),                  // stride
		(void*)offsetof(Vertex, texCoords)	           // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::UpdateBuffers()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
}