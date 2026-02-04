#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include <memory>
#include <filesystem>


#include "Shader.h"


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
        ShaderManager(std::vector<ShaderProgramFilePaths> shaderSources);
        Shader* getShaderPtr(std::string shaderName);

        std::vector<Shader*> getRawShaderPointers()
        {
            std::vector<Shader*> rawShaderPointers;
            rawShaderPointers.reserve(shaders.size()); 

            for (auto& [key, shaderPtr] : shaders)
            {
                rawShaderPointers.push_back(shaderPtr.get()); 
            }

            return rawShaderPointers;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

};

}

#endif