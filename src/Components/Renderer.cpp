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

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Renderer::RenderAll(Camera* camera)
{
	for (int i = renderers.size() - 1; i >= 0; i--)
		renderers[i]->Render(camera);
}

void Renderer::Render(Camera* camera)
{
	glUseProgram(material->GetProgramID());

	material->SetMat4x4("MVP", camera->GetRenderMatrix() * GetParent()->transform->GetMatrix4x4());

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::UpdateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned short), &mesh->indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}