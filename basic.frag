#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;
  
uniform vec3 uViewPos; 
uniform vec3 uLightColor;

uniform sampler2D uDiffMap1;
uniform sampler2D uSpecMap1;

void main() {
    vec3 lightDir = normalize(light.position - chFragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (theta > light.cutOff) {
        float distance = length(light.position - chFragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient
        vec3 ambient = light.ambient * texture(uDiffMap1, chUV).rgb;

        // diffuse
        vec3 norm = normalize(chNormal);
        vec3 lightDir = normalize(light.position - chFragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(uDiffMap1, chUV).rgb * uLightColor;

        // specular
        vec3 viewDir = normalize(uViewPos - chFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = light.specular * spec * texture(uSpecMap1, chUV).rgb * uLightColor;

        //ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        diffuse *= intensity;
        specular *= intensity;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    } else {
        FragColor = vec4(light.ambient * texture(uDiffMap1, chUV).rgb, 1.0);
    }
    
    //FragColor = texture(uDiffMap1, chUV) * vec4(ambient + diffuse, 1.0);
    //FragColor += texture(uSpecMap1, chUV) * vec4(specular, 1.0);
}

