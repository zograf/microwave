#include "Render.h"
#include "Shader.h"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

int Render::do_render() {
    if (create_window())
        return 1;

    load_shaders();
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    float vertices[] = {
        //X    Y    Z       R    G    B    A
        0.25, 0.5, 0.75, 1.0, 0.0, 0.0, 0.0, //Crveni trougao - Prednji
        -0.25, 0.5, 0.75, 1.0, 0.0, 0.0, 0.0,
        0.0, -0.5, 0.75, 1.0, 0.0, 0.0, 0.0,

        0.25, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, //Plavi trougao - Zadnji
        -0.25, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 0.0
    };
    unsigned int stride = (3 + 4) * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    render_loop();
    
    clean_up();
    
    return 0;
}

void Render::render_loop() {
    // UNIFORME
    
    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(unified_shader, "uM");

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int viewLoc = glGetUniformLocation(unified_shader, "uV");

    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(unified_shader, "uP");
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++
    glUseProgram(unified_shader);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
    glBindVertexArray(VAO);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
        }
        // Rotiranje levo desno
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, -1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        // Rotiranje gore dole
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(-1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        // Zumiranje
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            model = glm::scale(model, glm::vec3(0.9, 0.9, 0.9));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Render::load_shaders() {
    unified_shader = Shader::create_shader("basic.vert", "basic.frag");
}

void Render::clean_up() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unified_shader);
    
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

