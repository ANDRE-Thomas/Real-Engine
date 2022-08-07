#include "Rendering/Model.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

#include "Log.h"

#pragma region Static

Model* Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::Error("Error while loading model:");
		Log::Error(importer.GetErrorString());

		return nullptr;
	}

	return new Model(path, scene);
}

#pragma endregion Static

Model::Model(std::string path, const aiScene* scene)
{
	this->path = path;
	ProcessNode(scene->mRootNode, scene);
}

std::vector<Mesh> Model::GetMeshes()
{
	return meshes;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::vector<Texture> textures;

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

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	aiString textureName;
	std::string texturePath;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		mat->GetTexture(type, i, &textureName);
		texturePath = path.parent_path().string() + "/" + textureName.C_Str();

		auto pair = modelTextures.find(texturePath);
		if (pair != modelTextures.end())
		{
			textures.push_back(pair->second);
			continue;
		}

		GLuint textureID = TextureFromFile(texturePath.c_str());
		if (textureID == NULL)
			continue;

		Texture texture;
		texture.textureID = textureID;
		texture.name = std::string(textureName.C_Str()).substr(0, std::string(textureName.C_Str()).find_last_of(".")); //Remove file extension from name
		texture.type = typeName;

		textures.push_back(texture);
		modelTextures.insert({ texturePath, texture });
	}

	return textures;
}

GLuint Model::TextureFromFile(std::string texturePath)
{
	int width, height, nrComponents;
	stbi_uc* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLuint textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		return textureID;
	}
	else
	{
		Log::Error("Failed to load texture: " + texturePath);
		stbi_image_free(data);

		return NULL;
	}
}