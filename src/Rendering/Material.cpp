#include "Rendering/Material.h"

Material::Material(std::string shaderPath, std::string shaderName)
{
	programID =  LoadShaders(shaderPath + shaderName + ".vert", shaderPath + shaderName + ".frag");

	if (programID == NULL)
	{
		Log::Error("Error while loading shaders, reverting to default shader");

		programID = LoadShaders("res/shaders/default.vert", "res/shaders/default.frag");

		if (programID == NULL)
			throw std::runtime_error("Couln't load any shaders for material with shader: " + shaderName);
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

GLuint Material::LoadShaders(std::string vertexFilePath, std::string  fragmentFilePath)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	}
	else
	{
		Log::Error("Impossible to open " + vertexFilePath + ".Are you in the right directory ");
		return NULL;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}
	else
	{
		Log::Error("Impossible to open " + fragmentFilePath + ".Are you in the right directory ");
		return NULL;
	}

	GLint compilationResult = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	Log::Message("Compiling shader : " + vertexFilePath);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compilationResult);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);

		Log::Error("Error while compiling shader " + vertexFilePath + ":");
		Log::Error(&vertexShaderErrorMessage[0]);
		return NULL;
	}

	// Compile Fragment Shader
	Log::Message("Compiling shader : " + fragmentFilePath);
	char const* FragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compilationResult);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);

		Log::Error("Error while compiling shader " + fragmentFilePath + ":");
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

		Log::Error("Error while linking program for shaders " + fragmentFilePath + " and " + vertexFilePath + ":");
		Log::Error(&programErrorMessage[0]);
		return NULL;
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}