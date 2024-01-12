#include "Render.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Door.h"

void Render::setup_defaults(const Shader& unified_shader) {
    projection_p = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
    projection_o = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    glm::vec3 uViewPos = glm::inverse(view)[3];

    unified_shader.use();
    unified_shader.setVec3("uViewPos", uViewPos);
    unified_shader.setMat4("uP", projection_p);
    unified_shader.setMat4("uV", view);
    unified_shader.setVec3("light.color", 1, 1, 0.7);
    unified_shader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
    unified_shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    unified_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    unified_shader.setFloat("light.constant", 1.0f);
    unified_shader.setFloat("light.linear", 0.9f);
    unified_shader.setFloat("light.quadratic", 0.32f);
    unified_shader.setVec3("light.position", 0.2f, 0.14f, 0.12f);
    unified_shader.setVec3("light.direction", glm::vec3(-0.3, -3.0, 0.1));
    unified_shader.setFloat("light.cutOff", glm::cos(glm::radians(45.0f)));
    unified_shader.setFloat("light.outerCutOff", glm::cos(glm::radians(55.0f)));
}

int Render::do_render(const Shader& unified_shader) {
    setup_defaults(unified_shader);
    render_loop(unified_shader);

    clean_up();
    return 0;
}

void Render::render_loop(const Shader& unified_shader) {
    Model plate("res/microwave-plate.obj");
    Model microwave("res/microwave.obj");
    Door door = Door();

    glm::mat4 microwave_model = glm::mat4(1.0f);
    
    glm::mat4 plate_model = glm::mat4(1.0f);
    float ptx = 25.0f;
    float pty = -47.0f;
    float ptz = 30.0f;
    plate_model = glm::scale(plate_model, glm::vec3(1.5, 1.5, 1.5));
    plate_model = glm::scale(plate_model, glm::vec3(0.003, 0.003, 0.003));
    plate_model = glm::translate(plate_model, glm::vec3(ptx, pty, ptz));

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    int degree_counter = 0;
    while (!glfwWindowShouldClose(window)) {
        handle_input(unified_shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unified_shader.setMat4("uV", view);

        if (is_running) {
            plate_model = glm::rotate(plate_model, glm::radians(3.0f), glm::vec3(0, 1, 0));
            unified_shader.setVec3("light.color", 1, 1, 0.7);
        }
        else {
            unified_shader.setVec3("light.color", 0, 0, 0);
        }

        if (is_open_animation) {
            degree_counter += 1;
            door.rotate(false);
            if (degree_counter > 18) {
                degree_counter = 0;
                is_open_animation = false;
            }
        }
        if (is_close_animation) {
            degree_counter += 1;
            door.rotate(true);
            if (degree_counter > 18) {
                degree_counter = 0;
                is_close_animation = false;
            }
        }

        // Models
        unified_shader.setBool("isModel", true);
        unified_shader.setFloat("alpha", 1.0);
        unified_shader.setMat4("uM", microwave_model);
        microwave.Draw(unified_shader);

        unified_shader.setMat4("uM", plate_model);
        plate.Draw(unified_shader);

        // Objects
        door.draw(unified_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Render::handle_input(const Shader& unified_shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        unified_shader.setMat4("uP", projection_p);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        unified_shader.setMat4("uP", projection_o);
    }
    // Rotiranje levo desno
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    // Rotiranje levo desno
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    // Rotiranje gore dole
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    // Zumiranje
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        view = glm::scale(view, glm::vec3(1.1, 1.1, 1.1));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        view = glm::scale(view, glm::vec3(0.9, 0.9, 0.9));
        glm::vec3 uViewPos = glm::inverse(view)[3];
        unified_shader.setVec3("uViewPos", uViewPos);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (!is_close_animation && !is_open) {
            is_open_animation = true;
            is_running = false;
            is_open = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (!is_open_animation && is_open) {
            is_close_animation = true;
            is_open = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!is_open) is_running = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        is_running = false;
    }
}


void Render::clean_up() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
}

int Render::create_window() {
    if (!glfwInit()) {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, "[Mikrotalasna]", NULL, NULL);

    if (window == NULL) {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    return 0;
}

void draw_microwave() {
    
}