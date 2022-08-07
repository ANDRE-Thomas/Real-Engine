#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Rendering/Mesh.h"
#include <vector>
#include <string>
#include <filesystem>
#include <map>

class Model
{
public:
	static Model* LoadModel(std::string path);

	std::vector<Mesh> GetMeshes();

private:
	std::filesystem::path path;
	std::vector<Mesh> meshes;
	std::map<std::filesystem::path, Texture> modelTextures;

	Model(std::string, const aiScene* scene);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(std::string texturePath);
};