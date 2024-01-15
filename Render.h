#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class Render {
private:
    int window_width = 1400;
    int window_height = 900;
    
    float cam_x = 2.0f;
    float cam_y = 0.0f;
    float cam_z = 0.0f;
    
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    bool is_running = false;
    bool is_open_animation = false;
    bool is_close_animation = false;
    bool is_open = false;

    glm::mat4 view;
    glm::mat4 projection_o;
    glm::mat4 projection_p;

    void handle_input(const Shader &unified_shader, const Shader &light_shader);
    void render_loop(const Shader &unified_shader, const Shader &light_shader);
    void setup_defaults(const Shader &unified_shader, const Shader &light_shader);
    int do_render(const Shader &unified_shader, const Shader &light_shader);
    
    int create_window();
    void clean_up();

public:
    GLFWwindow* window;
    
    Render() {
        if (create_window()) return;
        const Shader unified_shader = Shader("basic.vert", "basic.frag");
        const Shader light_shader = Shader("light.vert", "light.frag");
        do_render(unified_shader, light_shader);
    }
};
