#include "ShaderManager.h"


namespace gfx {


ShaderManager::ShaderManager(std::vector<ShaderPaths> shaderSources)
{

    for (const auto& shaderSource : shaderSources)
    {
        Shader compiledShader = Shader(shaderSource.fragmentShader.string(), shaderSource.vertexShader.string(), shaderSource.setName);

        shaders[shaderSource.setName] = std::make_unique<Shader>(compiledShader);
    }

}

Shader* ShaderManager::getShaderPtr(std::string shaderName)
{
    return shaders[shaderName].get();
}

}