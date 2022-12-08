#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>
#include <filesystem>
#include <map>

#include "Real Engine/Rendering//Texture.h"
#include "Real Engine/Rendering/Mesh.h"

class Model
{
public:
	static Model* LoadModel(std::string path, bool loadTextures);

	std::vector<Mesh> GetMeshes();

private:
	std::filesystem::path path;
	std::vector<Mesh> meshes;
	std::map<std::filesystem::path, Texture> modelTextures;

	Model(std::string, const aiScene* scene, bool loadTextures);

	void ProcessNode(aiNode* node, const aiScene* scene, bool loadTextures);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, bool loadTextures);

	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};