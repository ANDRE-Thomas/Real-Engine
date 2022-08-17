#include "Components/Renderer.h"

#pragma region Static

std::vector<Renderer*> Renderer::renderers;

void Renderer::RenderAll(IRenderingViewPoint* viewPoint)
{
	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(viewPoint, renderers[i]->material);
}

void Renderer::RenderAll(IRenderingViewPoint* viewPoint, Material* material)
{
	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(viewPoint, material);
}

void Renderer::RenderAll(IRenderingViewPoint* viewPoint, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters)
{
	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(viewPoint, renderers[i]->material, SetMaterialParameters);
}

void Renderer::RenderAll(IRenderingViewPoint* viewPoint, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters, Material* material)
{
	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(viewPoint, material, SetMaterialParameters);
}

#pragma endregion

Renderer::Renderer(Model* model)
{
	this->model = model;
	this->material = Material::GetMaterial("res/internals/shaders/", "default");

	UpdateGeometryBuffers();

	renderers.push_back(this);
}

Renderer::Renderer(Model* model, Material* material) : Renderer(model)
{
	this->material = material;
}

Renderer::~Renderer()
{
	auto result = std::find(renderers.begin(), renderers.end(), this);
	if (result != renderers.end())
		renderers.erase(result);

	if (model != nullptr)
		delete(model);

	glDeleteBuffers(nBuffer, &VBO[0]);
	glDeleteBuffers(nBuffer, &EBO[0]);
	glDeleteVertexArrays(nBuffer, &VAO[0]);
}

void Renderer::Render(IRenderingViewPoint* viewPoint, Material* material, std::function<void(Material*, IRenderingViewPoint*, Renderer*)> SetMaterialParameters)
{
	glUseProgram(material->GetProgramID());

	if (SetMaterialParameters != nullptr)
		SetMaterialParameters(material, viewPoint, this);

	std::vector<Mesh> meshes = model->GetMeshes();
	for (size_t i = 0; i < nBuffer; i++)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);

		glDrawElements(GL_TRIANGLES, (GLsizei)meshes[i].indices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Renderer::UpdateGeometryBuffers()
{
	std::vector<Mesh> meshes = model->GetMeshes();

	if (VAO.size() != 0)
		glDeleteBuffers(VAO.size(), &VAO[0]);

	if (VBO.size() != 0)
		glDeleteBuffers(VBO.size(), &VBO[0]);

	if (EBO.size() != 0)
		glDeleteBuffers(EBO.size(), &EBO[0]);

	nBuffer = (int)meshes.size();
	VAO = std::vector<GLuint>(nBuffer);
	VBO = std::vector<GLuint>(nBuffer);
	EBO = std::vector<GLuint>(nBuffer);

	glGenVertexArrays(nBuffer, &VAO[0]);
	glGenBuffers(nBuffer, &VBO[0]);
	glGenBuffers(nBuffer, &EBO[0]);

	for (size_t i = 0; i < nBuffer; i++)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, meshes[i].vertices.size() * sizeof(Vertex), &meshes[i].vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i].indices.size() * sizeof(unsigned short), &meshes[i].indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}
}