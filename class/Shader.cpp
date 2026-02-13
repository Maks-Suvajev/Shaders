#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace gfx {

	Shader::Shader(const std::string& fragmentShaderPath, const std::string& vertexShaderPath, const std::string uniqueShaderName)
	: modelMatrixLocation(glUniformLocationLoadError),
	  viewMatrixLocation(glUniformLocationLoadError),
	  projectionMatrixLocation(glUniformLocationLoadError)
	{
		std::string vertexShaderCode = loadShaderCode(vertexShaderPath); 
		std::string fragmentShaderCode = loadShaderCode(fragmentShaderPath);

		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());

		compileShaderProgram(vertexShader, fragmentShader);

		useProgram();

		loadEachFileShaderVariables(vertexShaderCode, fragmentShaderCode);

		initialiseMvpMatrices();

        shaderName = uniqueShaderName;
	}

	void Shader::initialiseMvpMatrices()
	{
		loadMvpMatricesLocations();
		initialiseMvpMatricesValues();
	}

	void Shader::initialiseMvpMatricesValues()
	{
		// Initiliase all MVP matrices with identity matrices
		updateModelMatrixValue(glm::mat4(1.0f));
		updateViewMatrixValue(glm::mat4(1.0f));
		updateProjectionMatrixValue(glm::mat4(1.0f));
	}

	void Shader::checkShaderCompilation(GLuint shaderID)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);

            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; // Replace with logging module
            #endif

		}
	}

	std::string Shader::loadShaderCode(const std::string& shaderPath)
	{
		std::string shaderCode;
		std::ifstream shaderFile(shaderPath);

		if (!shaderFile.is_open())
		{
			throw std::ios_base::failure("Failed to open fragment shader: " + shaderPath);
		}

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream shaderStream;	

		try
		{
			shaderStream << shaderFile.rdbuf();

			shaderFile.close();

			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::SHADER::READING FILE: " << e.what() << std::endl;
            #else
                (void)e;
            #endif
		}

		return shaderCode;
	}

	GLuint Shader::compileShader(GLenum shaderType, char const * shaderCode)
	{
		GLuint shaderObject;

		shaderObject = glCreateShader(shaderType);

		glShaderSource(shaderObject, 1, &shaderCode, NULL);

		glCompileShader(shaderObject);

		checkShaderCompilation(shaderObject);

		return shaderObject;
	}

	void Shader::compileShaderProgram(GLuint vertexShader, GLuint fragmentShader)
	{
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

            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
            #endif
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	// Pull the uniform locations for the transformation matrices Model-View-Projection (MVP)
	void Shader::loadMvpMatricesLocations()
	{
		modelMatrixLocation = glGetUniformLocation(shaderID, modelMatrixUniformName);

		if	(modelMatrixLocation == glUniformLocationLoadError)
		{
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::\"model\" transformation matrix uniform not found in linked shader program." << std::endl;
            #endif
		}

		viewMatrixLocation = glGetUniformLocation(shaderID, viewMatrixUniformName);

		if	(viewMatrixLocation == glUniformLocationLoadError)
		{
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::\"view\" transformation matrix uniform not found in linked shader program." << std::endl;
            #endif
		}

		projectionMatrixLocation = glGetUniformLocation(shaderID, projectionMatrixUniformName);

		if	(projectionMatrixLocation == glUniformLocationLoadError)
		{
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "ERROR::\"projection\" transformation matrix uniform not found in linked shader program." << std::endl;
            #endif
        }
	}

	GLint Shader::getUniformLocation(const char * const name)
	{
			GLint uniformLocation = glGetUniformLocation(shaderID, name);

			if	(uniformLocation == glUniformLocationLoadError)
			{
                #ifdef ENABLE_DEBUG_MESSAGES
				    std::cout << "ERROR::\"model\" transformation matrix uniform not found in linked shader program." << std::endl;
                #endif
			}

			return uniformLocation;
	}

	bool Shader::updateModelMatrixValue(const glm::mat4& value)
	{
        static bool firstWrite = true;

        if (!firstWrite)
        {
            if (value == modelMatrixCache)
            {
                return true;
            }
            
            if (updateUniformValue(modelMatrixUniformName, value))
            {
                modelMatrixCache = value;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
		    updateUniformValue(modelMatrixUniformName, value);

            firstWrite = false;

            return true;

        }
	}

	bool Shader::updateViewMatrixValue(const glm::mat4& value)
	{

        static bool firstWrite = true;

        if (!firstWrite)
        {
            if (value == viewMatrixCache)
            {
                return true;
            }
            
            if (updateUniformValue(viewMatrixUniformName, value))
            {
                viewMatrixCache = value;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            updateUniformValue(viewMatrixUniformName, value);

            firstWrite = false;

            return true;
        }
	}

	bool Shader::updateProjectionMatrixValue(const glm::mat4& value)
	{
        static bool firstWrite = true;

        if (!firstWrite)
        {
            if (value == projectionMatrixCache)
            {
                return true;
            }
            
            if (updateUniformValue(projectionMatrixUniformName, value))
            {
                projectionMatrixCache = value;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            updateUniformValue(projectionMatrixUniformName, value);

            firstWrite = false;
            
            return true;
        }
	}

	void Shader::loadShaderUniformVariables(const std::string& shaderCode)
	{
		std::istringstream shaderCodeStream(shaderCode);
		std::string currLine;
		std::vector<std::string> lines;

		bool uniformDetected = false;
		bool uniformTypeSet = false;
		bool uniformValueSet = false;
		bool skip = false; // Skip other token checks if token has already been assigned

		GlslUniform uniform;

		while (std::getline(shaderCodeStream, currLine, ';')) {

			std::istringstream lineParse(currLine); // Create stream for line
			std::string currToken; 

			uniformDetected = false;
			uniformTypeSet = false;
			uniformValueSet = false;

			while(lineParse >> currToken)
			{
				skip = false;

				if (currToken == "uniform")
				{
					uniformDetected = true;
					skip = true;
				}

				if (uniformDetected && !uniformTypeSet && !skip)
				{
					uniform.typeString = currToken;

                    #ifdef ENABLE_DEBUG_MESSAGES
					    std::cout << "DEBUG::Uniform type string = " << uniform.typeString << std::endl;
                    #endif

					uniformTypeSet = true;
					skip = true;
				}
				else if (uniformDetected && !uniformValueSet && !skip)
				{
					uniform.uniformName = currToken;
					uniformValueSet = true;

                    #ifdef ENABLE_DEBUG_MESSAGES
					    std::cout << "DEBUG::Uniform type name = " << uniform.uniformName << std::endl;
                    #endif

					storeUniform(uniform);

					uniform = GlslUniform{}; //reset
				}
			}
		}
	}

	void Shader::loadEachFileShaderVariables(const std::string& VertShaderCode, const std::string& FragShaderCode)
	{
		loadShaderUniformVariables(VertShaderCode);
		loadShaderUniformVariables(FragShaderCode);
	}

	void Shader::storeUniform(const GlslUniform &uniform)
	{
		// need to check if keywords "model", "view" or "projection" are in uniform name.

		auto toLower = [](char c){ return std::tolower(static_cast<unsigned char>(c)); }; // Convert char to lowercase

		auto lowerCaseUniformNameView = uniform.uniformName | std::views::transform(toLower);
        
        #ifdef ENABLE_DEBUG_MESSAGES                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		    std::cout << "DEBUG::Storing uniform: " << uniform.uniformName << std::endl;
        #endif

		if (std::ranges::search(lowerCaseUniformNameView,  std::string_view("model")).begin() != lowerCaseUniformNameView.end())
		{
			matrixUniforms.model = uniform;

            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "DEBUG::Stored as model uniform." << std::endl;
            #endif
		}
		else if (std::ranges::search(lowerCaseUniformNameView, std::string_view("view")).begin() != lowerCaseUniformNameView.end())
		{
			matrixUniforms.view = uniform;

            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "DEBUG::Stored as view uniform." << std::endl;
            #endif
		}
		else if (std::ranges::search(lowerCaseUniformNameView, std::string_view("projection")).begin() != lowerCaseUniformNameView.end())
		{
			matrixUniforms.projection = uniform;
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "DEBUG::Stored as projection uniform." << std::endl;
            #endif
		}
		else
		{
            #ifdef ENABLE_DEBUG_MESSAGES
			    std::cout << "DEBUG::Stored as non-MVP uniform." << std::endl;
            #endif

			nonTransformUniforms.push_back(uniform);
		}
	}
}

