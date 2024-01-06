#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

class Render {
private:
    unsigned int unified_shader = 0;
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
