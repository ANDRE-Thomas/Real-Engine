#include "Real Engine/Rendering/Model.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

#include <utility>
#include <assimp/postprocess.h>

#include "Real Engine/Core/Log.h"

std::vector<std::pair<aiTextureType, std::string>> textureTypesToLoad = {
	{aiTextureType_DIFFUSE, "texture_diffuse"},
	{aiTextureType_SPECULAR, "texture_specular"},
	{aiTextureType_AMBIENT, "texture_ambient"},
	{aiTextureType_NORMALS , "texture_normals"}
};

#pragma region Static

Model* Model::LoadModel(std::string path, bool loadTextures)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::Error("Error while loading model:");
		Log::Error(importer.GetErrorString());

		return nullptr;
	}

	return new Model(path, scene, loadTextures);
}

#pragma endregion Static

Model::Model(std::string path, const aiScene* scene, bool loadTextures)
{
	this->path = path;
	ProcessNode(scene->mRootNode, scene, loadTextures);
}

std::vector<Mesh> Model::GetMeshes()
{
	return meshes;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, bool loadTextures)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, loadTextures));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, loadTextures);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, bool loadTextures)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	Vertex vertex{};
	vec3 vector3{};
	vec2 vector2{};
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.position = vector3;

		vector3.x = mesh->mNormals[i].x;
		vector3.y = mesh->mNormals[i].y;
		vector3.z = mesh->mNormals[i].z;
		vertex.normal = vector3;

		vector3.x = mesh->mTangents[i].x;
		vector3.y = mesh->mTangents[i].y;
		vector3.z = mesh->mTangents[i].z;
		vertex.tangent = vector3;

		if (mesh->mTextureCoords[0])
		{
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vector2;
		}
		else
			vertex.texCoords = vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0 && loadTextures)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		for (size_t i = 0; i < textureTypesToLoad.size(); i++)
			LoadMaterialTextures(material, textureTypesToLoad[i].first, textureTypesToLoad[i].second);
	}

	return Mesh(vertices, indices);
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	aiString textureName;
	std::string texturePath;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		mat->GetTexture(type, static_cast<GLsizei>(i), &textureName);
		texturePath = path.parent_path().string() + "/" + textureName.C_Str();

		auto pair = modelTextures.find(texturePath);
		if (pair != modelTextures.end())
			continue;

		try
		{
			std::string name = std::string(textureName.C_Str()).substr(0, std::string(textureName.C_Str()).find_last_of(".")); //Remove file extension from name
			Texture texture = Texture::LoadTexture(texturePath.c_str(), name);

			modelTextures.insert({ texturePath, texture });
		}
		catch (const std::runtime_error& exception)
		{
			Log::Error(exception.what());
			continue;
		}
	}
}