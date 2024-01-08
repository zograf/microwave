#include "Render.h"
#include "Shader.hpp"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"

int Render::do_render() {
    if (create_window())
        return 1;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    render_loop();

    clean_up();

    return 0;
}

void Render::render_loop() {
    Shader unified_shader = Shader("basic.vert", "basic.frag");

    Model microwave("res/microwave.obj");
    Model plate("res/microwave-plate.obj");
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++

    float cam_x = 2.0f;
    float cam_y = 0.0f;
    float cam_z = 0.0f;

    //Render petlja
    unified_shader.use();
    unified_shader.setVec3("uViewPos", 0, 0.5, 1);
    unified_shader.setVec3("light.color", 1, 1, 0.7);
    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.1f,
                                             100.0f);
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    unified_shader.setMat4("uP", projectionP);
    glm::mat4 view = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 2.0f, 0.0f));
    unified_shader.setMat4("uV", view);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 plate_model = glm::mat4(1.0f);
    glm::mat4 door_model = glm::mat4(1.0f);
    plate_model = glm::scale(plate_model, glm::vec3(1.5, 1.5, 1.5));
    float ptx = 25.0f;
    float pty = -47.0f;
    float ptz = 30.0f;

    plate_model = glm::scale(plate_model, glm::vec3(0.003, 0.003, 0.003));
    plate_model = glm::translate(plate_model, glm::vec3(ptx, pty, ptz));

    door_model = glm::rotate(door_model, glm::radians(90.0f), glm::vec3(0, 1, 0));

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


    // Transparent pane
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


    glClearColor(0.5, 0.5, 0.5, 1.0);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    bool is_running = false;
    bool is_open_animation = false;
    bool is_close_animation = false;
    bool is_open = false;

    int degree_counter = 0;
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            unified_shader.setMat4("uP", projectionP);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            unified_shader.setMat4("uP", projectionO);
        }
        // Rotiranje levo desno
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        }
        // Rotiranje levo desno
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        }
        // Rotiranje gore dole
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            view = glm::rotate(view, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        // Zumiranje
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            view = glm::scale(view, glm::vec3(1.1, 1.1, 1.1));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            view = glm::scale(view, glm::vec3(0.9, 0.9, 0.9));
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
            is_running = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            is_running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unified_shader.setMat4("uV", view);
        unified_shader.setMat4("uM", model);
        unified_shader.setBool("isModel", true);
        unified_shader.setFloat("alpha", 1.0);

        microwave.Draw(unified_shader);

        if (is_running) {
            plate_model = glm::rotate(plate_model, glm::radians(3.0f), glm::vec3(0, 1, 0));
            unified_shader.setVec3("light.color", 1, 1, 0.7);
        }
        else {
            unified_shader.setVec3("light.color", 0, 0, 0);
        }

        if (is_open_animation) {
            degree_counter += 1;
            door_model = glm::translate(door_model, glm::vec3(-0.53f, -0.23f, 0.48f));
            door_model = glm::rotate(door_model, glm::radians(5.0f), glm::vec3(0, -1, 0));
            door_model = glm::translate(door_model, glm::vec3(0.53f, 0.23f, -0.48f));
            if (degree_counter > 18) {
                degree_counter = 0;
                is_open_animation = false;
            }
        }
        if (is_close_animation) {
            degree_counter += 1;
            door_model = glm::translate(door_model, glm::vec3(-0.53f, -0.23f, 0.48f));
            door_model = glm::rotate(door_model, glm::radians(5.0f), glm::vec3(0, 1, 0));
            door_model = glm::translate(door_model, glm::vec3(0.53f, 0.23f, -0.48f));
            if (degree_counter > 18) {
                degree_counter = 0;
                is_close_animation = false;
            }
        }

        unified_shader.setMat4("uM", plate_model);
        plate.Draw(unified_shader);


        unified_shader.setBool("isModel", false);
        unified_shader.setVec3("objectColor", glm::vec3(0.0, 0.0, 1.0));
        unified_shader.setMat4("uM", door_model);

        glBindVertexArray(VAO);
        unified_shader.setFloat("alpha", 0.3);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 2);
        unified_shader.setFloat("alpha", 1.0);
        unified_shader.setVec3("objectColor", glm::vec3(0.0, 0.0, 0.0));
        glDrawArrays(GL_TRIANGLES, 6 * 2, 6 * 4);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
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
