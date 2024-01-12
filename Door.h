#pragma once
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.hpp"

class Door {
private:
    unsigned int VAO;
    unsigned int VBO;

    glm::mat4 door_model;
public:
    Door() {
        door_model = glm::mat4(1.0f);
        float door[] = {
            // Front face
            -0.53f, -0.23f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.25f, -0.23f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.23f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.25f, 0.23f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.53f, 0.23f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.53f, -0.23f, 0.5f, 0.0f, 0.0f, 1.0f,

            0.25f, -0.23f, 0.48f, 0.0f, 0.0f, -1.0f,
            -0.53f, -0.23f, 0.48f, 0.0f, 0.0f, -1.0f,
            -0.53f, 0.23f, 0.48f, 0.0f, 0.0f, -1.0f,
            -0.53f, 0.23f, 0.48f, 0.0f, 0.0f, -1.0f,
            0.25f, 0.23f, 0.48f, 0.0f, 0.0f, -1.0f,
            0.25f, -0.23f, 0.48f, 0.0f, 0.0f, -1.0f,

            // Left face
            -0.53f, -0.23f, 0.48f, -1.0f, 0.0f, 0.0f,
            -0.53f, -0.23f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.53f, 0.23f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.53f, 0.23f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.53f, 0.23f, 0.48f, -1.0f, 0.0f, 0.0f,
            -0.53f, -0.23f, 0.48f, -1.0f, 0.0f, 0.0f,

            // Right face
            0.25f, -0.23f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.25f, -0.23f, 0.48f, 1.0f, 0.0f, 0.0f,
            0.25f, 0.23f, 0.48f, 1.0f, 0.0f, 0.0f,
            0.25f, 0.23f, 0.48f, 1.0f, 0.0f, 0.0f,
            0.25f, 0.23f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.25f, -0.23f, 0.5f, 1.0f, 0.0f, 0.0f,

            // Top face
            -0.53f, 0.23f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.23f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.23f, 0.48f, 0.0f, 1.0f, 0.0f,
            0.25f, 0.23f, 0.48f, 0.0f, 1.0f, 0.0f,
            -0.53f, 0.23f, 0.48f, 0.0f, 1.0f, 0.0f,
            -0.53f, 0.23f, 0.5f, 0.0f, 1.0f, 0.0f,

            // Bottom face
            0.25f, -0.23f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.53f, -0.23f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.53f, -0.23f, 0.48f, 0.0f, -1.0f, 0.0f,
            -0.53f, -0.23f, 0.48f, 0.0f, -1.0f, 0.0f,
            0.25f, -0.23f, 0.48f, 0.0f, -1.0f, 0.0f,
            0.25f, -0.23f, 0.5f, 0.0f, -1.0f, 0.0f,
        };
        unsigned int stride = (3 + 3) * sizeof(float);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(door), door, GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        door_model = glm::rotate(door_model, glm::radians(90.0f), glm::vec3(0, 1, 0));
    }

    void draw(const Shader &unified_shader);
    void rotate(bool backwards);
};
