#include "Rendering/Material.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Log.h>
#include <stdexcept>
#include <filesystem>

#pragma region static

static const std::filesystem::path shadersArtefactsDirectory = "artefacts/shaders/";

std::map<std::string, Material*> Material::materialsMap;

Material* Material::GetMaterial(std::string materialPath, std::string materialName)
{
	std::string key = materialPath + materialName;

	if (materialsMap.find(key) == materialsMap.end())
		materialsMap.insert({ key, new Material(materialPath, materialName) });

	return materialsMap[key];
}

GLuint Material::LoadProgram(std::string materialPath, std::string materialName)
{
	std::string vertexCode = LoadCodeFromFile(materialPath + materialName + ".vert");
	std::string fragmentCode = LoadCodeFromFile(materialPath + materialName + ".frag");
	xxh::hash64_t codeHash = xxh::xxhash<64>(vertexCode + fragmentCode);

	std::string matID = materialPath + materialName;
	matID.erase(std::remove(matID.begin(), matID.end(), '/'), matID.end());
	matID.erase(std::remove(matID.begin(), matID.end(), '\\'), matID.end());

	GLuint loadResult = LoadProgramFromFile(matID, codeHash);
	if (loadResult != NULL)
		return loadResult;

	if (vertexCode.empty() || fragmentCode.empty())
	{
		Log::Error("One or more shader file was empty for material " + materialName + " aborting compilation");
		return NULL;
	}

	Log::Message("Compiling shader: " + materialName + "...");
	GLuint programID = CompileProgramFromShadersCode(vertexCode, fragmentCode);

	if (programID == NULL)
		return NULL;

	if (SaveProgramToFile(programID, matID, codeHash))
		Log::Message("Material " + materialName + " saved to file.");
	else
		Log::Warning("Error while saving material " + materialName + " to file.");

	return programID;
}

GLuint Material::LoadProgramFromFile(std::string materialID, xxh::hash64_t codehash)
{
	std::string materialDataFilename = (std::filesystem::current_path() / shadersArtefactsDirectory).string() + materialID;

	if (!std::filesystem::is_regular_file(materialDataFilename + ".shaderbin") || !std::filesystem::is_regular_file(materialDataFilename + ".shadermeta"))
		return NULL;

	GLuint programID = glCreateProgram();

	// Load binary from file
	std::ifstream binaryInput(materialDataFilename + ".shaderbin", std::ios::binary);
	std::istreambuf_iterator<char> startIt(binaryInput), endIt;
	std::vector<char> buffer(startIt, endIt);  // Load file
	binaryInput.close();

	// Load metadata from file
	std::ifstream metaInput(materialDataFilename + ".shadermeta", std::ios::app);
	GLenum format = 0;
	metaInput.read((char*)&format, sizeof(GLenum));
	xxh::hash64_t materialCodeHash = 0;
	metaInput.read((char*)&materialCodeHash, sizeof(xxh::hash64_t));
	metaInput.close();

	if (materialCodeHash != codehash)
	{
		Log::Warning("Material code was changed since last compilation, recompiling material...");
		return NULL;
	}

	// Install shader binary
	glProgramBinary(programID, format, buffer.data(), buffer.size());

	// Check for success/failure
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		Log::Error("Error while loading material from file");
		return NULL;
	}

	return programID;
}

bool Material::SaveProgramToFile(GLuint programID, std::string materialID, xxh::hash64_t codeHash)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	if (formats < 1)
	{
		Log::Warning("Driver does not support any binary formats to save program binary.");
		return false;
	}

	// Get the binary length
	GLint length = 0;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	//Get buffer
	std::vector<GLubyte> buffer(length);
	GLenum format = 0;
	glGetProgramBinary(programID, length, NULL, &format, buffer.data());

	if (!std::filesystem::exists(shadersArtefactsDirectory))
		std::filesystem::create_directories(shadersArtefactsDirectory);

	std::string shaderDataPath = (std::filesystem::current_path() / shadersArtefactsDirectory).string();

	// Write the binary to a file.
	std::ofstream outBinary(shaderDataPath + materialID + ".shaderbin", std::ofstream::binary | std::ofstream::trunc);
	outBinary.write(reinterpret_cast<char*>(buffer.data()), length);
	outBinary.close();

	// Write the metadata to a file.
	std::ofstream outMetadata(shaderDataPath + materialID + ".shadermeta", std::ios::trunc);
	outMetadata.write((char*)&format, sizeof(GLenum));
	outMetadata.write((char*)&codeHash, sizeof(xxh::hash64_t));
	outMetadata.close();

	return true;
}

GLuint Material::CompileProgramFromShadersCode(std::string vertexCode, std::string fragmentCode)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint compilationResult = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	char const* vertexSourcePointer = vertexCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compilationResult);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);

		Log::Error("Error while compiling shader: ");
		Log::Error(&vertexShaderErrorMessage[0]);
		return NULL;
	}

	// Compile Fragment Shader
	char const* FragmentSourcePointer = fragmentCode.c_str();
	glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compilationResult);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);

		Log::Error("Error while compiling shader: ");
		Log::Error(&fragmentShaderErrorMessage[0]);
		return NULL;
	}

	// Link the program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &compilationResult);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);

		Log::Error("Error while linking program: ");
		Log::Error(&programErrorMessage[0]);
		return NULL;
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

std::string Material::LoadCodeFromFile(std::string filePath)
{
	std::string shaderCode;
	std::ifstream shaderCodeStream(filePath, std::ios::in);
	if (shaderCodeStream.is_open())
	{
		std::stringstream sstr;
		sstr << shaderCodeStream.rdbuf();
		shaderCode = sstr.str();
		shaderCodeStream.close();

		return shaderCode;
	}
	else
	{
		Log::Error("Impossible to open file " + filePath);
		return std::string();
	}
}

#pragma endregion static

Material::Material(std::string materialPath, std::string materialName)
{
	programID = LoadProgram(materialPath, materialName);

	if (programID == NULL)
	{
		Log::Error("Error while compiling material: " + materialName + ", reverting to default material");

		programID = LoadProgram("res/shaders/", "default");

		if (programID == NULL)
			throw std::runtime_error("Couln't load any shaders for material: " + materialName);
	}
}

Material::~Material()
{
	if (programID != NULL)
		glDeleteProgram(programID);
}

GLint Material::GetProgramID()
{
	return programID;
}

void Material::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Material::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Material::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Material::SetMat4x4(const std::string& name, mat4x4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}