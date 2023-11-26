#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define WINDOW_TITLE    "[Microwave]"

unsigned int initLibraries();
static unsigned loadImageToTexture(const char* filePath);

GLFWwindow* window;

int main(void) {
    unsigned int error = initLibraries();
    if (error != 0)
        return error;

    unsigned int textureShader = createShader("texture.vert", "texture.frag");
    unsigned int mainFrameShader = createShader("mainframe.vert", "mainframe.frag");

    unsigned int VAO[8];
    unsigned int VBO[8];

    glGenVertexArrays(8, VAO);
    glGenBuffers(8, VBO);

    float textureBackground[] = {

        // Background
         1, -1,        1.0, 0.0,
        -1, -1,        0.0, 0.0,
        -1,  1,        0.0, 1.0,

        -1,  1,        0.0, 1.0,
         1, -1,        1.0, 0.0,
         1,  1,        1.0, 1.0,

         // Food
         -0.1, -0.5,       1.0, 0.0,
         -0.55, -0.5,        0.0, 0.0,
         -0.55, -0.2,        0.0, 1.0,

        -0.55,  -0.2,       0.0, 1.0,
        -0.1, -0.5,        1.0, 0.0,
        -0.1,  -0.2,       1.0, 1.0,

    };

    unsigned int textureStride = 4 * sizeof(float);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureBackground), textureBackground, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned background = loadImageToTexture("res/background.png");
    unsigned food = loadImageToTexture("res/food.png");

    // Background texture
    glBindTexture(GL_TEXTURE_2D, background);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Food texture
    glBindTexture(GL_TEXTURE_2D, food);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Main frame
    float mainFrame[] = {

        // Main white
        -0.8f, -0.8f,       0.9, 0.9, 0.9, 1.0,
        0.6f, -0.8f,        0.9, 0.9, 0.9, 1.0,
        0.6f, 0.4f,         0.9, 0.9, 0.9, 1.0,

        0.6f, 0.4f,         0.9, 0.9, 0.9, 1.0,
        -0.8f, 0.4f,        0.9, 0.9, 0.9, 1.0,
        -0.8f, -0.8f,       0.9, 0.9, 0.9, 1.0,

        // Side panel
        0.1f, 0.35f,        0.3, 0.3, 0.3, 1.0,
        0.55f, 0.35f,       0.3, 0.3, 0.3, 1.0,
        0.55f, -0.75f,      0.3, 0.3, 0.3, 1.0,

        0.55f, -0.75f,      0.3, 0.3, 0.3, 1.0,
        0.1f, -0.75f,       0.3, 0.3, 0.3, 1.0,
        0.1f, 0.35f,        0.3, 0.3, 0.3, 1.0,

        // 3D side
        0.6f, -0.8f,        0.7, 0.7, 0.7, 1.0,
        0.8f, -0.6f,        0.7, 0.7, 0.7, 1.0,
        0.6f, 0.4f,         0.7, 0.7, 0.7, 1.0,

        0.6f, 0.4f,         0.7, 0.7, 0.7, 1.0,
        0.8f, 0.6f,         0.7, 0.7, 0.7, 1.0,
        0.8f, -0.6f,        0.7, 0.7, 0.7, 1.0,

        // 3D top
        -0.8f, 0.4f,        1.0, 1.0, 1.0, 1.0,
        0.6f, 0.4f,         1.0, 1.0, 1.0, 1.0,
        0.8f, 0.6f,         1.0, 1.0, 1.0, 1.0,

        -0.6f, 0.6f,        1.0, 1.0, 1.0, 1.0,
        0.8f, 0.6f,         1.0, 1.0, 1.0, 1.0,
        -0.8f, 0.4f,        1.0, 1.0, 1.0, 1.0,

        // Shadow bottom
        -0.7f, -0.9f,       0.0, 0.0, 0.0, 0.4,
        0.7f, -0.9f,        0.0, 0.0, 0.0, 0.4,
        -0.8f, -0.8f,       0.0, 0.0, 0.0, 0.4,

        -0.8f, -0.8f,       0.0, 0.0, 0.0, 0.4,
        0.6f, -0.8f,        0.0, 0.0, 0.0, 0.4,
        0.7f, -0.9f,        0.0, 0.0, 0.0, 0.4,

        // Shadow side
        0.6f, -0.8f,        0.0, 0.0, 0.0, 0.4,
        0.7f, -0.9f,        0.0, 0.0, 0.0, 0.4,
        0.8f, -0.6f,        0.0, 0.0, 0.0, 0.4,

        0.8f, -0.6f,        0.0, 0.0, 0.0, 0.4,
        0.7f, -0.9f,        0.0, 0.0, 0.0, 0.4,
        0.9f, -0.67f,        0.0, 0.0, 0.0, 0.4,

        // Inside left
		-0.5, 0.3,        0.4, 0.4, 0.4, 1.0,
		-0.5, -0.4,       0.4, 0.4, 0.4, 1.0,
		-0.75, -0.7,      0.4, 0.4, 0.4, 1.0,

	    -0.75, -0.7,      0.4, 0.4, 0.4, 1.0,
		-0.5, 0.3,        0.4, 0.4, 0.4, 1.0,
		-0.75,  0.3,      0.4, 0.4, 0.4, 1.0,

        // Inside bottom
		0.05, -0.4,       0.6, 0.6, 0.6, 1.0,
		-0.5, -0.4,       0.6, 0.6, 0.6, 1.0,
		-0.75, -0.7,      0.6, 0.6, 0.6, 1.0,

	    -0.75, -0.7,      0.6, 0.6, 0.6, 1.0,
		0.05, -0.4,       0.6, 0.6, 0.6, 1.0,
		0.05,  -0.7,      0.6, 0.6, 0.6, 1.0,
        
        // Inside back
		0.05, -0.4,       0.2, 0.2, 0.2, 1.0,
		-0.5, -0.4,       0.2, 0.2, 0.2, 1.0,
		-0.5, 0.3,        0.2, 0.2, 0.2, 1.0,

	    -0.5,  0.3,       0.2, 0.2, 0.2, 1.0,
		0.05, -0.4,       0.2, 0.2, 0.2, 1.0,
		0.05,  0.3,       0.2, 0.2, 0.2, 1.0,
    };
    unsigned int mainFrameStride = 6 * sizeof(float);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mainFrame), mainFrame, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, mainFrameStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, mainFrameStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Glass 
    float glass[] = {

        // Glass
         0.05, -0.7,       0.678, 0.847, 0.902, 0.6,
         -0.75, -0.7,      0.678, 0.847, 0.902, 0.6, 
         -0.75, 0.3,      0.678, 0.847, 0.902, 0.6,

        -0.75,  0.3,      0.678, 0.847, 0.902, 0.6,
        0.05, -0.7,        0.678, 0.847, 0.902, 0.6,
        0.05,  0.3,       0.678, 0.847, 0.902, 0.6,
    };
    unsigned int glassStride = 6 * sizeof(float);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glass), glass, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, glassStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, glassStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Door strip
    float doorStrip[] = {

        // Door strip
         0.05, -0.7,
         -0.75, -0.7,
         -0.75, 0.3,
        0.05,  0.3,
         0.05, -0.7
    };
    unsigned int doorStride = 2 * sizeof(float);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorStrip), doorStrip, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, doorStride, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Door handle
    float doorHandle[] = {

         0.04, -0.5,    0.0, 0.0, 0.0, 1.0,
         0.07, -0.5,   0.0, 0.0, 0.0, 1.0,
         0.07, 0.1,    0.0, 0.0, 0.0, 1.0,

         0.04,  -0.5,     0.0, 0.0, 0.0, 1.0,
         0.07, 0.1,     0.0, 0.0, 0.0, 1.0,
         0.04, 0.1,     0.0, 0.0, 0.0, 1.0,
    };
    unsigned int doorHandleStride = 6 * sizeof(float);

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorHandle), doorHandle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, doorHandleStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, doorHandleStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	glfwSwapInterval(1);

    bool isClosed = true;
    bool isBlending = true;

    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) isClosed = false;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) isClosed = true;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            isBlending = false;
            glDisable(GL_BLEND);
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			isBlending = true;
			glEnable(GL_BLEND);
        }

        // Background
        glUseProgram(textureShader);
        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, background);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        // Main frame
        glUseProgram(mainFrameShader);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 54);
        glBindVertexArray(0);
        glUseProgram(0);

        // Food
        glUseProgram(textureShader);
        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, food);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        if (isClosed) {
			// Glass
			glUseProgram(mainFrameShader);
			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			glUseProgram(0);

			// Door
			glLineWidth(5.0f);
			glUseProgram(mainFrameShader);
			glBindVertexArray(VAO[3]);
			glDrawArrays(GL_LINE_STRIP, 0, 5);
			glBindVertexArray(0);
			glUseProgram(0);
			glLineWidth(1.0f);

			// Door Handle
			glUseProgram(mainFrameShader);
			glBindVertexArray(VAO[4]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			glUseProgram(0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteTextures(1, &background);
    //glDeleteBuffers(1, &VBO);
    //glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(textureShader);
    glfwTerminate();
    return 0;
}

unsigned int initLibraries() {

    if (!glfwInit()) {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

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

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL) {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
			case 1: InternalFormat = GL_RED; break;
			case 3: InternalFormat = GL_RGB; break;
			case 4: InternalFormat = GL_RGBA; break;
			default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    } else {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

