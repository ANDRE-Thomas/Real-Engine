#pragma once

#include <GL/glew.h>

#include <string>
#include <filesystem>

class Texture
{
public:
	static Texture LoadTexture(std::filesystem::path path, std::string name);
	static GLuint TextureFromFile(std::string path);

public:
	Texture(GLuint textureID, std::string name);

	GLuint GetID();
	std::string GetName();
	void Release();

private:
	GLuint textureID;
	std::string name;
};