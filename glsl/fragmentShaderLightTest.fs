#version 330 core

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

#define NR_POINT_LIGHTS 1

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirLight(vec3 normal, vec3 viewDirection);
vec3 CalcPointLight(PointLight pointLightInstance, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 CalcSpotLight(SpotLight spotLightInstance, vec3 normal, vec3 fragPosition, vec3 viewDirection);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 result = CalcDirLight(norm, viewDir);

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(normal, lightDir), 0.0); // if dot < 0 max will return 0.0 instead 

    vec3 reflectDir = reflect(-lightDir, normal); // Have to negate because its pointing from frag to light currently
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

    vec3 ambient = directionalLight.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = directionalLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = (directionalLight.specular * spec) * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight pointLightInstance, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 ambient = pointLightInstance.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(vec3(pointLightInstance.position) - fragPosition);

    float diff = max(dot(normal, lightDir), 0.0); // if dot < 0 max will return 0.0 instead 

    vec3 diffuse = pointLightInstance.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectrDir = reflect(-lightDir, normal); // Have to negate because its pointing from frag to light currently

    float spec = pow(max( dot(viewDirection, reflectrDir), 0.0), material.shininess);

    vec3 specular = (pointLightInstance.specular * spec) * vec3(texture(material.specular, TexCoords));

    float distance = length(pointLightInstance.position - fragPosition);
    float attenuation = 1.0f / (pointLightInstance.constant + (pointLightInstance.linear * distance) + (pointLightInstance.quadratic * (distance * distance)));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight spotLightInstance, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 ambient = spotLightInstance.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(spotLightInstance.position - fragPosition);

    float diff = max(dot(normal, lightDir), 0.0); // if dot < 0 max will return 0.0 instead 

    vec3 diffuse = spotLightInstance.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectrDir = reflect(-lightDir, normal); // Have to negate because its pointing from frag to light currently

    float spec = pow(max(dot(viewDirection, reflectrDir), 0.0), material.shininess);

    vec3 specular = (spotLightInstance.specular * spec) * vec3(texture(material.specular, TexCoords));

    float theta = dot(lightDir, normalize(-spotLightInstance.direction));
    float epsilon = spotLightInstance.cutOff - spotLightInstance.outerCutOff;
    float intensity = clamp((theta - spotLightInstance.outerCutOff)/ epsilon, 0.0, 1.0);

    diffuse  *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}