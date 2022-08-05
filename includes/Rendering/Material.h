#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Log.h>
#include <stdexcept>

class Material
{
public:
	Material(std::string  shaderPath, std::string shaderName);
	~Material();

	GLint GetProgramID();

private:
	GLint programID;

	GLuint LoadShaders(std::string  vertex_file_path, std::string  fragment_file_path);
};