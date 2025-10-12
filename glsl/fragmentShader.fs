#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textures[32]; // Need to read the max value of texture units 



void main()
{
	FragColor = mix(texture(textures[0], TexCoord), texture(textures[1], TexCoord), 0.5); // Definitely needs to be fixed, need to add variable to toggle particular texture unit
}