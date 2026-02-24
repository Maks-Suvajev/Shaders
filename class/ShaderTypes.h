#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#include <string>
#include <filesystem>

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

#endif