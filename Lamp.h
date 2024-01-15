#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.hpp"

class Lamp {
private:
    std::vector<float> data;
    unsigned int VAO;
    unsigned int VBO;

public:
    glm::mat4 lamp_model;
    Lamp(bool isLeft, bool isUp = false) {
        lamp_model = glm::mat4(1.0f);
        data = std::vector<float>();
        
        int num_slices = 50;
        int num_stacks = 25;
        float radius = 0.05f;
        
        generate_half_sphere(radius, num_slices, num_stacks);
        
        unsigned int stride = (3 + 3) * sizeof(float);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (!isUp) {
            lamp_model = glm::rotate(lamp_model, glm::radians(90.0f), glm::vec3(0, 0, -1));
            if (!isLeft) lamp_model = glm::translate(lamp_model, glm::vec3(0.18f, 0.5f, -0.55f));
            else lamp_model = glm::translate(lamp_model, glm::vec3(0.18f, 0.5f, -0.30f));
            lamp_model = glm::scale(lamp_model, glm::vec3(0.6f, 0.6f, 0.6f));
        } else {
            lamp_model = glm::rotate(lamp_model, glm::radians(180.0f), glm::vec3(0, 0, -1));
            lamp_model = glm::translate(lamp_model, glm::vec3(0.0f, -0.24f, 0.12f));
            lamp_model = glm::scale(lamp_model, glm::vec3(0.6f, 0.6f, 0.6f));
        }
    }

    void generate_half_sphere(float radius, int numSlices, int numStacks);
    void draw(const Shader &light_shader, glm::vec3 color);
};
