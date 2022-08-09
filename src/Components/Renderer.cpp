#include "Components/Renderer.h"

#include "Components/DirectionalLight.h"

#pragma region Static

std::vector<Renderer*> Renderer::renderers;
GLuint Renderer::lightsSSBO;

void Renderer::RenderAll(Camera* camera)
{
	UpdateGlobalBuffers();

	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(camera);
}

void Renderer::UpdateGlobalBuffers()
{
	if (lightsSSBO == NULL)
	{
		glGenBuffers(1, &lightsSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightsSSBO);
	}

	std::vector<LightInfos> lightInfos = Light::GetAllLightsInfos();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightInfos.size() * sizeof(LightInfos), &lightInfos[0], GL_STATIC_READ);
}

#pragma endregion Static

Renderer::Renderer(Model* model)
{
	this->model = model;
	this->material = Material::GetMaterial("res/shaders/", "default");

	UpdateBuffers();

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

#include "Components/Transform.h"

void Renderer::Render(Camera* camera)
{
	GLuint programID = material->GetProgramID();
	glUseProgram(programID);

	material->SetVec3("viewPosition", camera->transform->position);
	material->SetMat4x4("MVP", camera->GetRenderMatrix() * GetParent()->transform->GetMatrix4x4());
	material->SetMat4x4("modelMat", GetParent()->transform->GetMatrix4x4());
	material->SetMat3x3("normalMat", transpose(inverse(mat3x3(GetParent()->transform->GetMatrix4x4()))));

	std::vector<Texture> textures = material->GetTextures();
	for (size_t j = 0; j < textures.size(); j++)
	{
		glActiveTexture(GL_TEXTURE0 + j);

		material->SetInt("material." + textures[j].GetName(), j); //Bind uniform to texture location
		glBindTexture(GL_TEXTURE_2D, textures[j].GetID()); //Bind texture to texture location
	}

	glActiveTexture(GL_TEXTURE0);

	std::vector<Mesh> meshes = model->GetMeshes();
	for (size_t i = 0; i < nBuffer; i++)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);

		glDrawElements(GL_TRIANGLES, (GLsizei)meshes[i].indices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Renderer::UpdateBuffers()
{
	std::vector<Mesh> meshes = model->GetMeshes();

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