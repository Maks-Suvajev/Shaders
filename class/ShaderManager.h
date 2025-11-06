#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "Shader.h"
#include <unordered_map>
#include <memory>

namespace gfx {

    struct ShaderSource
    {
        std::string shaderName;
        std::string vertexShader;
        std::string fragmentShader;
    };


	class ShaderManager
	{
		public:
			ShaderManager(std::vector<ShaderSource> shaderSources);

		private:
            std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

	};

}

#endif