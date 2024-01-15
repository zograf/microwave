#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 chFragPos = vec3(model * vec4(aPos, 1.0));
    vec3 chNormal = mat3(transpose(inverse(model))) * inNormal;

    gl_Position = projection * view * vec4(chFragPos, 1.0);
    
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
}