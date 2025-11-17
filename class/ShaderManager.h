#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include <memory>
#include <filesystem>


#include "Shader.h"


namespace gfx {

struct ShaderFilenameStrings
{
    std::string setName;
    std::string vertexShader;
    std::string fragmentShader;
};

// Define type to hold shader paths
struct ShaderPaths
{
    std::string setName;
    std::filesystem::path vertexShader;
    std::filesystem::path fragmentShader;
};


class ShaderManager
{
    public:
        ShaderManager(std::vector<ShaderPaths> shaderSources);
        Shader* getShaderPtr(std::string shaderName);

        std::vector<Shader*> getRawShaderPointers()
        {
            std::vector<Shader*> rawShaderPointers;
            rawShaderPointers.reserve(shaders.size()); // optional, for efficiency

            for (auto& [key, shaderPtr] : shaders)
            {
                rawShaderPointers.push_back(shaderPtr.get()); // get raw pointer without transferring ownership
            }

            return rawShaderPointers;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

};

}

#endif