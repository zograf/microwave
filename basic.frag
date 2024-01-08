#version 330 core

struct Light {
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

uniform Material material;
uniform Light light;

uniform vec3 uViewPos;
uniform vec3 objectColor;
uniform float alpha;
uniform bool isModel;

out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;


void main() {
    vec3 lightDir = normalize(light.position - chFragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (theta > light.cutOff) {
        float distance = length(light.position - chFragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient
        vec3 ambient = light.ambient;
        if (isModel)    ambient *= texture(material.uDiffMap1, chUV).rgb;
        else            ambient *= objectColor;

        // diffuse
        vec3 norm = normalize(chNormal);
        vec3 lightDir = normalize(light.position - chFragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        
        vec3 diffuse = light.diffuse * diff * light.color;
        if (isModel)    diffuse *= texture(material.uDiffMap1, chUV).rgb;
        else            diffuse *= objectColor;

        // specular
        vec3 viewDir = normalize(uViewPos - chFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        
        vec3 specular = light.specular * spec * light.color;
        if (isModel)    specular *= texture(material.uSpecMap1, chUV).rgb;
        else            specular *= objectColor;

        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        FragColor = vec4(ambient + diffuse + specular, alpha);
    } else {
        if (isModel)    FragColor = vec4(light.ambient * texture(material.uDiffMap1, chUV).rgb, alpha);
        else            FragColor = vec4(light.ambient * objectColor, alpha);
    }
}

