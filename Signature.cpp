#include "Signature.h"

void Signature::draw(const Shader& signature_shader, float window_width, float window_height) {
    glViewport(0, window_height - 19, 287, 19);
    signature_shader.use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, signature);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glViewport(0, 0, window_width, window_height);
}
