

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define FRAG_SHADER_PATH "C:\\Users\\Maks-\\OneDrive\\Desktop\\OpenGL\\Project\\OpenGLTestProject\\OpenGLTestProject\\fragmentShader.fs"
#define VERT_SHADER_PATH "C:\\Users\\Maks-\\OneDrive\\Desktop\\OpenGL\\Project\\OpenGLTestProject\\OpenGLTestProject\\vertexShader.vs"

class Shader
{
	public:
		Shader(const std::string &fragmentShaderPath, const std::string &vertexShaderPath);

		unsigned int getShaderID()
		{
			return shaderID;
		}


		void checkShaderCompilation(GLuint shaderID);


	private:
		unsigned int shaderID;

};



#endif