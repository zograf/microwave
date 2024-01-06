#pragma once
#include <GL/glew.h>

class Shader {
    
public:
    unsigned static int compile_shader(GLenum type, const char* source);
    unsigned static int create_shader(const char* vsSource, const char* fsSource);
};
