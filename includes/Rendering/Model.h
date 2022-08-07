#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Rendering/Mesh.h"
#include <vector>
#include <string>

class Model
{
public:
	static Model* LoadModel(std::string path);

	std::vector<Mesh> GetMeshes();

private:
	std::vector<Mesh> meshes;
	std::string directory;

	Model(const aiScene* scene);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma);
};