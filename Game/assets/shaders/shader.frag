#version 440 core

in vec2 TextureCoord;
in vec3 aNormal;
in vec3 FragPos;
in float IsEdge;
out vec4 FragColor;

const int MAX_PHONG_LIGHTS = 4;
const float levels = 3.0;
const float rimAmount = 0.716;

struct Light
{
    //Light properties
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //Attenuation
    float constant;
    float linear;
    float quadratic;

};

struct Material
{
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 viewPos;
uniform int lightCount;
uniform Material material;
uniform Light phongLights[MAX_PHONG_LIGHTS];
uniform sampler2D textureSampler;


vec3 CalculatePhongLights()
{
    vec3 result                 = vec3(0,0,0);
    vec3 unitNormal             = normalize(aNormal);
    vec3 unitViewDirection      = normalize(viewPos - FragPos);
    // float rimDot                 = 1 - dot(unitViewDirection, unitNormal);

    for(int i = 0; i < lightCount; i++)
    {
        vec3 lightDir     = phongLights[i].lightPos - FragPos;
        float distance    = length(lightDir);
        vec3 unitLightDir = normalize(lightDir);    
        float attenuation = 1.0 / (phongLights[i].constant + phongLights[i].linear * distance + phongLights[i].quadratic * (distance * distance));

        float nDotL       = dot(unitNormal, unitLightDir);
        float brightness  = max(nDotL, 0.0);
        float level       = floor(brightness * levels);
        brightness        = level / levels; 

        vec3 reflectDir      = reflect(-unitLightDir, unitNormal);
        float specularFactor = dot(unitViewDirection, reflectDir);
        specularFactor       = max(specularFactor, 0.0);
        float dampedFactor   = pow(specularFactor, material.shininess);

        level          = floor(dampedFactor * levels);
        dampedFactor   = level / levels;

        vec3 ambient   = phongLights[i].ambient * material.ambient;
        vec3 specular  = (material.specular * dampedFactor) * phongLights[i].specular; 
        vec3 diffuse   = (brightness * material.diffuse * phongLights[i].diffuse) ;
  
        //Phong
        result +=  ( ambient + diffuse + specular) * attenuation;
    }
    
    return result;
}

void main()
{
    vec3 lightIntensity = CalculatePhongLights();
    if(IsEdge == 1.0)
    {
        FragColor = vec4(0.0,0.0,0.0,1.0);
    }
    else
    {
        FragColor = texture(textureSampler, TextureCoord) * vec4(lightIntensity, 1.0);
    }
}

