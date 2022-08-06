#pragma once

#include <map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "xxhash.hpp"

class Material
{
public:
	static Material* GetMaterial(std::string materialPath, std::string materialName);

private:
	static std::map<std::string, Material*> materialsMap;

	static GLuint LoadProgram(std::string materialPath, std::string materialName);

	static GLuint LoadProgramFromFile(std::string materialID, xxh::hash64_t codehash);
	static bool SaveProgramToFile(GLuint programID, std::string materialID, xxh::hash64_t codeHash);

	static GLuint CompileProgramFromShadersCode(std::string vertexFilePath, std::string  fragmentFilePath);
	static std::string LoadCodeFromFile(std::string filePath);

public:
	GLint GetProgramID();

private:
	GLint programID;

	Material(std::string materialPath, std::string materialName);
	~Material();
};