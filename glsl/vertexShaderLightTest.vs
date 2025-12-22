#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos; //Fragment position in world space

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));

    // Required for non-uniform scaling in order to keep normal perp. to surface - should be done on CPU side
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; 
    //Normal = aNormal;

    TexCoords = aTexCoords;
}