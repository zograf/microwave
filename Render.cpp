#include "Render.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Door.h"
#include "Lamp.h"
#include "Signature.h"

void Render::setup_defaults(const Shader& unified_shader, const Shader& light_shader) {
    projection_p = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
    projection_o = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    glm::vec3 uViewPos = glm::inverse(view)[3];
    active_projection = projection_p;

    light_shader.use();
    light_shader.setMat4("projection", projection_p);
    light_shader.setMat4("view", view);

    unified_shader.use();
    unified_shader.setVec3("uViewPos", uViewPos);
    unified_shader.setMat4("uP", projection_p);
    unified_shader.setMat4("uV", view);

    unified_shader.setVec3("spotLight.color", 0, 0, 0);
    unified_shader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
    unified_shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
    unified_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    unified_shader.setFloat("spotLight.constant", 0.5f);
    unified_shader.setFloat("spotLight.linear", 0.9f);
    unified_shader.setFloat("spotLight.quadratic", 0.32f);
    unified_shader.setVec3("spotLight.position", 0.2f, 0.14f, 0.12f);
    unified_shader.setVec3("spotLight.direction", glm::vec3(-0.3, -3.0, 0.1));
    unified_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(45.0f)));
    unified_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(55.0f)));

    // Ova je desna lampica
    unified_shader.setVec3("pointLights[0].color", 1, 1, 0);
    unified_shader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    unified_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    unified_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    unified_shader.setFloat("pointLights[0].constant", 0.0f);
    unified_shader.setFloat("pointLights[0].linear", 9.9f);
    unified_shader.setFloat("pointLights[0].quadratic", 9.32f);
    unified_shader.setVec3("pointLights[0].position", 0.52f, -0.18f, -0.55f);

    // Ova je leva lampica
    unified_shader.setVec3("pointLights[1].color", 1, 1, 0);
    unified_shader.setVec3("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
    unified_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    unified_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    unified_shader.setFloat("pointLights[1].constant", 0.0f);
    unified_shader.setFloat("pointLights[1].linear", 9.9f);
    unified_shader.setFloat("pointLights[1].quadratic", 9.32f);
    unified_shader.setVec3("pointLights[1].position", 0.52f, -0.18f, -0.3f);
}

int Render::do_render(const Shader& unified_shader, const Shader& light_shader) {
    setup_defaults(unified_shader, light_shader);
    render_loop(unified_shader, light_shader);

    clean_up();
    return 0;
}

void Render::render_loop(const Shader& unified_shader, const Shader& light_shader) {
    Shader signature_shader = Shader("signature.vert", "signature.frag");
    
    Model plate("res/microwave-plate.obj");
    Model microwave("res/microwave.obj");
    Model knob("res/knob.obj");
    Model slider("res/slider.obj");

    Door door = Door();
    Lamp lamp_right = Lamp(false);
    Lamp lamp_left = Lamp(true);
    Lamp lamp_inside = Lamp(true, true);

    Signature signature = Signature(TextureFromFile("signature.png", "res"));

    glm::mat4 microwave_model = glm::mat4(1.0f);

    glm::mat4 slider_model = glm::mat4(1.0f);
    float slider_x = 0.501f;
    float slider_y = 0.0f;
    float slider_z = -0.425f;
    slider_model = glm::translate(slider_model, glm::vec3(slider_x, slider_y, slider_z));
    slider_model = glm::scale(slider_model, glm::vec3(0.15, 0.15, 0.15));

    knob_model = glm::mat4(1.0f);
    float knob_x = 0.50f;
    float knob_y = 0.0f;
    float knob_z = -0.275f;
    knob_model = glm::translate(knob_model, glm::vec3(knob_x, knob_y, knob_z));
    knob_model = glm::scale(knob_model, glm::vec3(0.05, 0.05, 0.05));

    glm::mat4 plate_model = glm::mat4(1.0f);
    float ptx = 25.0f;
    float pty = -47.0f;
    float ptz = 30.0f;
    plate_model = glm::scale(plate_model, glm::vec3(1.5, 1.5, 1.5));
    plate_model = glm::scale(plate_model, glm::vec3(0.003, 0.003, 0.003));
    plate_model = glm::translate(plate_model, glm::vec3(ptx, pty, ptz));

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    int degree_counter = 0;
    double prev = glfwGetTime();

    glm::vec3 flashing_color = glm::vec3(0.1, 0.1, 0.0);
    glm::vec3 inside_color = glm::vec3(0.1, 0.1, 0.0);
    glm::vec3 finished_color = glm::vec3(1.0, 1.0, 0.0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handle_input(unified_shader, light_shader);

        unified_shader.setMat4("uV", view);
        unified_shader.setMat4("uP", active_projection);

        // LOGIC

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

        if (is_running) {
            double now = glfwGetTime();
            if (now - prev >= 1) {
                knob_model = glm::translate(knob_model, glm::vec3(0.0, 0.0, SLIDER_STEP));
                seconds--;
                prev = now;
            }

            if (seconds == 0) {
                is_running = false;
            }
        }
        else prev = glfwGetTime();

        if (is_running)
            unified_shader.setVec3("pointLights[0].color", glm::mix(glm::vec3(0.0f, 0.0f, 0.0f),
                                                                    glm::vec3(1.0f, 1.0f, 0.0f),
                                                                    abs(sin(glfwGetTime() * 10))));
        else unified_shader.setVec3("pointLights[0].color", 0.0, 0.0, 0.0);

        if (seconds == 0) unified_shader.setVec3("pointLights[1].color", 1.0, 1.0, 0.0);
        else unified_shader.setVec3("pointLights[1].color", 0.0, 0.0, 0.0);

        if (is_running) {
            plate_model = glm::rotate(plate_model, glm::radians(3.0f), glm::vec3(0, 1, 0));
            unified_shader.setVec3("spotLight.color", 1, 1, 0.7);
        }
        else {
            unified_shader.setVec3("spotLight.color", 0, 0, 0);
        }

        // DRAWING MODELS

        unified_shader.setBool("isModel", true);
        unified_shader.setFloat("alpha", 1.0);
        unified_shader.setMat4("uM", microwave_model);
        microwave.Draw(unified_shader);

        unified_shader.setMat4("uM", plate_model);
        plate.Draw(unified_shader);

        unified_shader.setMat4("uM", slider_model);
        slider.Draw(unified_shader);

        unified_shader.setMat4("uM", knob_model);
        knob.Draw(unified_shader);

        // LIGHT

        light_shader.use();
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", active_projection);

        if (seconds == 0) finished_color = glm::vec3(1.0f, 1.0f, 0.0f);
        else finished_color = glm::vec3(0.1f, 0.1f, 0.0f);
        lamp_left.draw(light_shader, finished_color);

        if (is_running) {
            inside_color = glm::vec3(1.0f, 1.0f, 0.0f);
            flashing_color = glm::mix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
                                      abs(sin(glfwGetTime() * 10)));
        }
        else {
            inside_color = glm::vec3(0.1f, 0.1f, 0.0f);
            flashing_color = glm::vec3(0.1f, 0.1f, 0.0f);
        }
        lamp_inside.draw(light_shader, inside_color);
        lamp_right.draw(light_shader, flashing_color);


        // SIGNATURE
        signature.draw(signature_shader, window_width, window_height);
        
        // DOOR
        unified_shader.use();
        door.draw(unified_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Render::handle_input(const Shader& unified_shader, const Shader& light_shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        active_projection = projection_p;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        active_projection = projection_o;
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

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && !is_running) {
        if (seconds < 20) {
            seconds += 1;
            knob_model = glm::translate(knob_model, glm::vec3(0.0, 0.0, -SLIDER_STEP));
        }
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !is_running) {
        if (seconds > 0) {
            knob_model = glm::translate(knob_model, glm::vec3(0.0, 0.0, SLIDER_STEP));
            seconds -= 1;
        }
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !is_running && !is_open && seconds > 0) {
        is_running = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        is_running = false;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(2);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        glDisable(GL_BLEND);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        glEnable(GL_BLEND);
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