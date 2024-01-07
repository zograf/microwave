#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "shader.hpp"

class Render {
private:
    int window_width = 1400;
    int window_height = 900;
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    void load_shaders();
    void render_loop();
    int create_window();
    void clean_up();

public:
    GLFWwindow* window;

    int do_render();
};
