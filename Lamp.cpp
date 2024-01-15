#include "Lamp.h"

#include <vector>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>

void Lamp::generate_half_sphere(float radius, int num_slices, int num_stacks) {
    data.clear();

    const float pi = glm::pi<float>();

    for (int i = 0; i < num_stacks; ++i) {
        float stack_angle_1 = pi / 2.0f * static_cast<float>(i) / num_stacks;
        float stack_angle_2 = pi / 2.0f * static_cast<float>(i + 1) / num_stacks;

        for (int j = 0; j < num_slices; ++j) {
            float slice_angle_1 = 2.0f * pi * static_cast<float>(j) / num_slices;
            float slice_angle_2 = 2.0f * pi * static_cast<float>(j + 1) / num_slices;

            glm::vec4 vertex1(radius * cos(slice_angle_1) * cos(stack_angle_2),
                              radius * sin(stack_angle_2),
                              radius * sin(slice_angle_1) * cos(stack_angle_2),
                              1.0f);

            glm::vec4 vertex2(radius * cos(slice_angle_2) * cos(stack_angle_2),
                              radius * sin(stack_angle_2),
                              radius * sin(slice_angle_2) * cos(stack_angle_2),
                              1.0f);

            glm::vec4 vertex3(radius * cos(slice_angle_1) * cos(stack_angle_1),
                              radius * sin(stack_angle_1),
                              radius * sin(slice_angle_1) * cos(stack_angle_1),
                              1.0f);

            glm::vec4 vertex4(radius * cos(slice_angle_2) * cos(stack_angle_1),
                              radius * sin(stack_angle_1),
                              radius * sin(slice_angle_2) * cos(stack_angle_1),
                              1.0f);

            glm::vec3 normal1 = glm::normalize(glm::vec3(vertex1.x, vertex1.y, vertex1.z));
            glm::vec3 normal2 = glm::normalize(glm::vec3(vertex2.x, vertex2.y, vertex2.z));
            glm::vec3 normal3 = glm::normalize(glm::vec3(vertex3.x, vertex3.y, vertex3.z));
            glm::vec3 normal4 = glm::normalize(glm::vec3(vertex4.x, vertex4.y, vertex4.z));

            data.push_back(vertex1.x);
            data.push_back(vertex1.y);
            data.push_back(vertex1.z);
            data.push_back(normal1.x);
            data.push_back(normal1.y);
            data.push_back(normal1.z);

            data.push_back(vertex2.x);
            data.push_back(vertex2.y);
            data.push_back(vertex2.z);
            data.push_back(normal2.x);
            data.push_back(normal2.y);
            data.push_back(normal2.z);

            data.push_back(vertex3.x);
            data.push_back(vertex3.y);
            data.push_back(vertex3.z);
            data.push_back(normal3.x);
            data.push_back(normal3.y);
            data.push_back(normal3.z);

            data.push_back(vertex2.x);
            data.push_back(vertex2.y);
            data.push_back(vertex2.z);
            data.push_back(normal2.x);
            data.push_back(normal2.y);
            data.push_back(normal2.z);

            data.push_back(vertex4.x);
            data.push_back(vertex4.y);
            data.push_back(vertex4.z);
            data.push_back(normal4.x);
            data.push_back(normal4.y);
            data.push_back(normal4.z);

            data.push_back(vertex3.x);
            data.push_back(vertex3.y);
            data.push_back(vertex3.z);
            data.push_back(normal3.x);
            data.push_back(normal3.y);
            data.push_back(normal3.z);
            
        }
    }
}

void Lamp::draw(const Shader& light_shader, glm::vec3 color) {
    //light_shader.setMat4("uM", lamp_model);
    light_shader.setMat4("model", lamp_model);
    light_shader.setVec4("lightColor", glm::vec4(color, 1.0));
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, data.size() / 6);
    glBindVertexArray(0);
}