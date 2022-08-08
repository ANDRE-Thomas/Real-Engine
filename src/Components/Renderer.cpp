#include "Components/Renderer.h"

#include "Components/DirectionalLight.h"

std::vector<Renderer*> Renderer::renderers;

Renderer::Renderer(Model* model, Material* material)
{
	this->model = model;
	this->material = material;

	UpdateBuffers();

	renderers.push_back(this);
}

Renderer::~Renderer()
{
	if (model != nullptr)
		delete(model);

	auto result = std::find(renderers.begin(), renderers.end(), this);
	if (result != renderers.end())
		renderers.erase(result);

	glDeleteBuffers(nBuffer, &VBO[0]);
	glDeleteBuffers(nBuffer, &EBO[0]);
	glDeleteVertexArrays(nBuffer, &VAO[0]);
}

void Renderer::RenderAll(Camera* camera)
{
	for (size_t i = renderers.size(); i-- > 0;)
		renderers[i]->Render(camera);
}

void Renderer::Render(Camera* camera)
{
	GLuint programID = material->GetProgramID();
	glUseProgram(programID);

	material->SetVec3("viewPosition", camera->transform->GetPosition());
	material->SetMat4x4("MVP", camera->GetRenderMatrix() * GetParent()->transform->GetMatrix4x4());
	material->SetMat4x4("modelMat", GetParent()->transform->GetMatrix4x4());
	material->SetMat3x3("normalMat", transpose(inverse(mat3x3(GetParent()->transform->GetMatrix4x4()))));

	DirectionalLight* mainLight = DirectionalLight::GetInstance();

	if (mainLight != nullptr)
	{
		material->SetVec3("mainLight.lightPosition", mainLight->GetParent()->transform->GetPosition());
		material->SetVec3("mainLight.lightColor", mainLight->lightColor);
	}

	std::vector<Mesh> meshes = model->GetMeshes();
	for (size_t i = 0; i < nBuffer; i++)
	{
		std::vector<Texture> textures = material->GetTextures();
		for (size_t j = 0; j < textures.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);

			material->SetInt("material." + textures[j].GetName(), j); //Bind uniform to texture location
			glBindTexture(GL_TEXTURE_2D, textures[j].GetID()); //Bind texture to texture location
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);

		glDrawElements(GL_TRIANGLES, (GLsizei)meshes[i].indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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