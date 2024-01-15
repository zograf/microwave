#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D uDiffMap1;
    sampler2D uSpecMap1;
};

#define NR_POINT_LIGHTS 2

uniform Material material;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 uViewPos;
uniform vec3 objectColor;
uniform float alpha;
uniform bool isModel;

out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(chNormal);
    vec3 viewDir = normalize(uViewPos - chFragPos);
    
    vec3 result = CalcSpotLight(spotLight, norm, chFragPos, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, chFragPos, viewDir);
    
    FragColor = vec4(result, alpha);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (theta > light.cutOff) {
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient
        vec3 ambient = light.ambient;
        if (isModel)    ambient *= texture(material.uDiffMap1, chUV).rgb;
        else            ambient *= objectColor;

        // diffuse
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 diffuse = light.diffuse * diff * light.color;
        if (isModel)    diffuse *= texture(material.uDiffMap1, chUV).rgb;
        else            diffuse *= objectColor;

        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 specular = light.specular * spec * light.color;
        if (isModel)    specular *= texture(material.uSpecMap1, chUV).rgb;
        else            specular *= objectColor;

        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    } else {
        if (isModel)    return (light.ambient * texture(material.uDiffMap1, chUV).rgb);
        else            return (light.ambient * objectColor);
    }
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.ambient;
    if (isModel)    ambient *= vec3(texture(material.uDiffMap1, chUV));
    else            ambient *= objectColor;
    
    vec3 diffuse = light.diffuse * diff * light.color;
    if (isModel)    diffuse *= vec3(texture(material.uDiffMap1, chUV));
    else            diffuse *= objectColor;
    
    vec3 specular = light.specular * spec * light.color;
    if (isModel)    specular *= vec3(texture(material.uSpecMap1, chUV));
    else            specular *= objectColor;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}