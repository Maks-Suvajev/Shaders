#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Ambient lighting calc
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting calc
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0); // if dot < 0 max will return 0.0 instead
    vec3 diffuse = diff * lightColor;

    // Specular lighting calc
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectrDir = reflect(-lightDir, norm); // Have to negate because its pointing from frag to light currently

    float spec = pow(max( dot(viewDir, reflectrDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}