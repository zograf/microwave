#include "Door.h"

void Door::draw(const Shader& unified_shader) {
    unified_shader.setBool("isModel", false);
    unified_shader.setVec3("objectColor", glm::vec3(0.0, 0.0, 1.0));
    unified_shader.setMat4("uM", door_model);
    unified_shader.setFloat("alpha", 0.3);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 2);
    unified_shader.setFloat("alpha", 1.0);
    unified_shader.setVec3("objectColor", glm::vec3(0.0, 0.0, 0.0));
    glDrawArrays(GL_TRIANGLES, 6 * 2, 6 * 4);
    glBindVertexArray(0);
}

void Door::rotate(bool backwards) {
    int y = -1;
    if (backwards) y = 1;
    
    door_model = glm::translate(door_model, glm::vec3(-0.53f, -0.23f, 0.48f));
    door_model = glm::rotate(door_model, glm::radians(5.0f), glm::vec3(0, y, 0));
    door_model = glm::translate(door_model, glm::vec3(0.53f, 0.23f, -0.48f));
}
