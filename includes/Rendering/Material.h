#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include <map>
#include <string>
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

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetMat4x4(const std::string& name, mat4x4 value);

private:
	GLint programID;

	Material(std::string materialPath, std::string materialName);
	~Material();
};