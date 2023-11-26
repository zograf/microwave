#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define WINDOW_TITLE    "[Microwave]"
#define PI              3.14159265358979323846


unsigned int initLibraries();
static unsigned loadImageToTexture(const char* filePath);

GLFWwindow* window;

int main(void) {
    unsigned int error = initLibraries();
    if (error != 0)
        return error;

    unsigned int backgroundTextureShader = createShader("backgroundtex.vert", "backgroundtex.frag");
    unsigned int textureShader = createShader("texture.vert", "texture.frag");
    unsigned int mainFrameShader = createShader("mainframe.vert", "mainframe.frag");
    unsigned int flashShader = createShader("flash.vert", "flash.frag");
    unsigned int lampShader = createShader("lamp.vert", "lamp.frag");
    unsigned int sliderShader = createShader("slider.vert", "slider.frag");

    unsigned int VAO[16];
    unsigned int VBO[16];

    glGenVertexArrays(16, VAO);
    glGenBuffers(16, VBO);

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

    unsigned int isOnTex = glGetUniformLocation(textureShader, "isOn");
    glUniform1i(isOnTex, 0);

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


    // Blinker LED
    float blinkerLED[] = {
        0.15, -0.67,      1.0, 0.5, 0.5, 1.0
    };

    unsigned int blinkerLEDStride = 6 * sizeof(float);
    unsigned int blinkParam = glGetUniformLocation(flashShader, "param");

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blinkerLED), blinkerLED, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, blinkerLEDStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, blinkerLEDStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Lamp
    const int CRES = 180;
    const int xOffset = -11;
    const int yOffset = 5;
    float circle[2 * CRES + 4];
    circle[0] = 0.0 + xOffset;
    circle[1] = 0.0 + yOffset;
    for (int i = 0; i <= CRES; i++) {
        circle[2 + 2 * i] = cos((3.141592 / 180) * (i * 360 / CRES)) + xOffset;
        circle[2 + 2 * i + 1] = sin((3.141592 / 180) * (i * 360 / CRES)) + yOffset;
    }

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.25, 0.25, 0.25, 0.25);
    glUseProgram(lampShader);
    unsigned int uRxLoc = glGetUniformLocation(lampShader, "uRx");
    unsigned int uRyLoc = glGetUniformLocation(lampShader, "uRy");
    unsigned int uColLoc = glGetUniformLocation(lampShader, "uCol");
    glUniform1f(uRxLoc, 0.03);
    glUniform1f(uRyLoc, 0.05);
    glUniform3f(uColLoc, 0.0, 0.0, 0.0);


    // Slider
    float sliderLine[] = {
         0.12, -0.5,
         0.53, -0.5,
    };

    unsigned int sliderLineStride = 2 * sizeof(float);

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sliderLine), sliderLine, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sliderLineStride, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float sliderKnob[] = {
		 0.12, -0.55,    1.0, 1.0, 0.0, 1.0,
         0.14, -0.55,   1.0, 1.0, 0.0, 1.0,
         0.14, -0.45,    1.0, 1.0, 0.0, 1.0,

         0.12,  -0.55,     1.0, 1.0, 0.0, 1.0,
         0.14, -0.45,     1.0, 1.0, 0.0, 1.0,
         0.12, -0.45,     1.0, 1.0, 0.0, 1.0,
    };

    unsigned int sliderKnobStride = 6 * sizeof(float);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sliderKnob), sliderKnob, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sliderKnobStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sliderKnobStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(sliderShader);
    unsigned int sliderOffset = glGetUniformLocation(sliderShader, "offset");
    float sliderXOffset = 0.0;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	glfwSwapInterval(1);

    bool isClosed = true;
    bool isBlending = true;
    bool isOn = false;

    int seconds = 0;

    double prev = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            isClosed = false;
            isOn = false;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) isClosed = true;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isClosed && seconds > 0) {
            isOn = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) isOn = false;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            isBlending = false;
            glDisable(GL_BLEND);
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			isBlending = true;
			glEnable(GL_BLEND);
        }

        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && !isOn) {
            sliderXOffset += 0.02;
            seconds++;
            if (sliderXOffset >= 0.4) sliderXOffset = 0.4;
            if (seconds >= 20) seconds = 20;
        }

        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !isOn) {
            sliderXOffset -= 0.02;
            seconds--;
            if (sliderXOffset <= 0.0) sliderXOffset = 0.0;
            if (seconds <= 0) seconds = 0;
        }

        if (isOn) {
			double now = glfwGetTime();
			if (now - prev >= 1) {
				sliderXOffset -= 0.02;
                if (sliderXOffset <= 0.0) sliderXOffset = 0.0;
                seconds--;
                if (seconds <= 0) seconds = 0;
				prev = now;
			}

            if(seconds == 0) {
                isOn = false;
            }
        } else prev = glfwGetTime();

        // Background
        glUseProgram(backgroundTextureShader);
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

        // Lamp
		glUseProgram(lampShader);
		glBindVertexArray(VAO[6]);
        if (isOn) glUniform3f(uColLoc, 0.7, 0.8, 0.2);
        else glUniform3f(uColLoc, 0.0, 0.0, 0.0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
		glBindVertexArray(0);
		glUseProgram(0);
        
        // Food
        glUseProgram(textureShader);
        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);

        if (isOn) glUniform1i(isOnTex, 1);
        else glUniform1i(isOnTex, 0);

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

        // Blinker LED
		glUseProgram(flashShader);
		glBindVertexArray(VAO[5]);

        if (isOn) glUniform1f(blinkParam, abs(sin(glfwGetTime() * 10)));
        else glUniform1f(blinkParam, 0.0);

		glPointSize(20);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
		glUseProgram(0);


		// Slider
		glLineWidth(8.0f);
		glUseProgram(mainFrameShader);
		glBindVertexArray(VAO[7]);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		glUseProgram(0);
		glBindVertexArray(0);
		glLineWidth(1.0f);

		glUseProgram(sliderShader);
		glBindVertexArray(VAO[8]);
		glUniform1f(sliderOffset, sliderXOffset);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);


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

