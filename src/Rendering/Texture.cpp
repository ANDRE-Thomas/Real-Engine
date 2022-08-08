#include "Rendering/Texture.h"

#include <stb_image.h>
#include <excpt.h>
#include "Log.h"

#pragma region Static

Texture Texture::LoadTexture(std::filesystem::path path, std::string name)
{
	GLuint textureID = TextureFromFile(path.string());

	if (textureID == NULL)
		throw std::runtime_error("Failed to load texture: " + path.string());

	return Texture(textureID, name);
}

GLuint Texture::TextureFromFile(std::string path)
{
	int width, height, nrComponents;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
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
		stbi_image_free(data);

		return NULL;
	}
}

#pragma endregion Static

Texture::Texture(GLuint textureID, std::string name)
{
	this->textureID = textureID;
	this->name = name;
}

void Texture::Release()
{
	if (textureID != NULL)
		glDeleteTextures(1, &textureID);
}

GLuint Texture::GetID()
{
	return textureID;
}

std::string Texture::GetName()
{
	return name;
}