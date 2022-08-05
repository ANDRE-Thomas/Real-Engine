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
	mat4 mvp = camera->GetRenderMatrix() * GetParent()->transform->GetMatrix4x4();

	glUseProgram(material->GetProgramID());

	// Send the transformation to the currently bound shader, in the "MVP" uniform
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : uvs
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_TRUE,            // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::UpdateBuffers()
{
	mvpID = glGetUniformLocation(material->GetProgramID(), "MVP");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(vec3), &mesh->vertices[0], GL_STATIC_DRAW);

	/*glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->uvs.size() * sizeof(vec2), &mesh->uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(vec3), &mesh->normals[0], GL_STATIC_DRAW);*/
}