#version 330 core

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light 
{
    vec4 lightSource;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Light light;
uniform vec3 viewPos;
uniform Material material;

void main()
{
    // Ambient lighting calc
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse lighting calc
    vec3 norm = normalize(Normal);

    vec3 lightDir;

    if (light.lightSource.w < 0.5)
    {
        lightDir = normalize(-vec3(light.lightSource));
    }
    else
    {
        lightDir = normalize(vec3(light.lightSource) - FragPos);
    }

    float diff = max(dot(norm, lightDir), 0.0); // if dot < 0 max will return 0.0 instead
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular lighting calc
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectrDir = reflect(-lightDir, norm); // Have to negate because its pointing from frag to light currently

    float spec = pow(max( dot(viewDir, reflectrDir), 0.0), material.shininess);
    vec3 specular = (light.specular * spec) * vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}