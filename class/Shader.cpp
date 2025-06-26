#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader(const std::string& fragmentShaderPath, const std::string& vertexShaderPath)
{
	std::string vertShaderCode, fragShaderCode;

	std::ifstream fragShaderFile(fragmentShaderPath);

	if (!fragShaderFile.is_open())
	{
		throw std::ios_base::failure("Failed to open fragment shader: " + fragmentShaderPath);
	}

	std::ifstream vertShaderFile(vertexShaderPath);

	if (!vertShaderFile.is_open())
	{
		throw std::ios_base::failure("Failed to open vertex shader: " + vertexShaderPath);
	}

	std::stringstream vertShaderStream, fragShaderStream;


	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();

		vertShaderFile.close();
		fragShaderFile.close();

		vertShaderCode = vertShaderStream.str();
		fragShaderCode = fragShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::READING FILE: " << e.what() << std::endl;
	}

	char const* vertShaderCodeFormatted = vertShaderCode.c_str(); // Needs to be a char const* for gl function inputs
	char const* fragShaderCodeFormatted = fragShaderCode.c_str();

	// vertexShader Setup
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //create vertex shader object

	glShaderSource(vertexShader, 1, &vertShaderCodeFormatted, NULL); //instantiate shader source code

	glCompileShader(vertexShader); //Compile shader

	// Check if compilation was successful
	checkShaderCompilation(vertexShader);

	// fragmentShader setup (Orange)
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCodeFormatted, NULL);
	glCompileShader(fragmentShader);

	// Check if compilation was successful
	checkShaderCompilation(fragmentShader);

	// Create the combined shader program
	shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);

	//Check linking
	int success;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	char infoLog[512];
	if (!success) {
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Delete individual shaders once they are linked with the shaderProgram
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


void Shader::checkShaderCompilation(GLuint shaderID)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}