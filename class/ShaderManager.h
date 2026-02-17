#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include <memory>
#include <filesystem>

#include "Shader.h"

// QT
#include <QOpenGLExtraFunctions>

namespace gfx {

constexpr char shaderModuleProjectName[] = "Shaders"; 
constexpr char shaderSourceFolderName[] = "glsl"; 

struct ShaderProgramFilenameStrings
{
    std::string setName;
    std::string vertexShader;
    std::string fragmentShader;
};

// Define type to hold shader paths
struct ShaderProgramFilePaths
{
    std::string setName;
    std::filesystem::path vertexShader;
    std::filesystem::path fragmentShader;
};

class ShaderManager
{
    public:
        ShaderManager(std::vector<ShaderProgramFilePaths> shaderSources, QOpenGLExtraFunctions* openGLFunctions);
        Shader* getShaderPtr(std::string shaderName);
        Shader* getShaderPtr(GLuint shaderID);
        GLuint getShaderID(std::string shaderName);
        void printAllShaderPrograms();

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

        QOpenGLExtraFunctions* m_openGLFunctions;

};

}

#endif