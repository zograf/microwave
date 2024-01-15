#pragma once
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.hpp"

class Signature {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int signature;
public:
    Signature(unsigned int s) {
        signature = s;
        float textureCords[] = {
            1.0, 1.0, 1.0, 0.0,
            -1.0, 1.0, 0.0, 0.0,
            -1.0, -1.0, 0.0, 1.0,
            
            -1.0, -1.0, 0.0, 1.0,
            1.0, -1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 0.0
        };

        unsigned int textureStride = (2 + 2) * sizeof(float);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCords), textureCords, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw(const Shader &signature_shader, float window_width, float window_height);
};
