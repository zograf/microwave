#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helpers.h"

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define WINDOW_TITLE    "[Microwave]"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
unsigned int initLibraries();

GLFWwindow* window;

int main(void) {
    unsigned int error = initLibraries();
    if (error != 0)
        return error;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag"); // Napravi objedinjeni sejder program

    float vertices[] = //Tjemena trougla koja sadrze sve informacije o njemu
    {
        //Podaci su poredani za nasu citkivost - racunar ne vidi ni razmake ni redove.
        //Moramo mu naknadno reci kako da intepretira ove podatke

        //Pozicija    |    Boja
        //X    Y       R    G    B    A
        0.25, 0.0,    1.0, 0.0, 0.0, 0.0, //prvo tjeme
        -0.25, 0.0,   0.0, 0.0, 1.0, 0.0, //drugo tjeme
        0.0, -0.5,     1.0, 1.0, 0.0, 0.0 //trece tjeme
    };
    unsigned int stride = (2 + 4) * sizeof(float); //Korak pri kretanju po podacima o tjemenima = Koliko mjesta u memoriji izmedju istih komponenti susjednih tjemena
    //U nasem slucaju XY (2) + RGBA (4) = 6

    //Vertex Array Object - Kontejner za VBO i pokazivace na njihove atribute
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //Generisi 1 kontejner na adresi od promjenljive "VAO"
    glBindVertexArray(VAO); //Povezi VAO za aktivni kontekst - Sve sto radimo ispod ovoga ce se odnositi na kontejner "VAO"

    //Vertex Buffer Object - Nase figure koje crtamo
    unsigned int VBO;
    glGenBuffers(1, &VBO); //Generisi 1 bafer sa adresom promjenljive "VBO" 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Povezi "VBO" za aktivni Array Buffer (on se koristi za VBO-eve)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Objasni gdje su podaci i za koji bafer
    //glBufferData(koji bafer, koliko podataka ima, adresa podataka, tip iscrtavanja (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW; optimizacioni parametar)

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0); //Objasni da su prva dva broja u tjemenu jedan atribut (u ovom slucaju pozicija)
    //glVertexAttribPointer(indeks pokazivaca, broj komponenti atributa, tip komponenti atributa, da li je potrebno normalizovati podatke (nama uvijek GL_FALSE), korak da bi dosao do iste komponente narednog tjemena, pomjeraj sa pocetka jednog tjemena do komponente za ovaj atribut - mora biti (void*))  
    glEnableVertexAttribArray(0); //Aktiviraj taj pokazivac i tako intepretiraj podatke
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float))); //Objasni da su preostala cetiri broja boja (preskacemo preko XY od pozicije, pa je pomjeraj 2 * sizeof(float))
    glEnableVertexAttribArray(1);

    //Postavili smo sta treba, pa te stvari iskljucujemo, da se naknadna podesavanja ne bi odnosila na njih i nesto poremetila
    //To radimo tako sto bindujemo 0, pa kada treba da nacrtamo nase stvari, samo ih ponovo bindujemo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++

    while (!glfwWindowShouldClose(window)) //Beskonacna petlja iz koje izlazimo tek kada prozor treba da se zatvori
    {
        //Unos od korisnika bez callback funckcije. GLFW_PRESS = Dugme je trenutno pritisnuto. GLFW_RELEASE = Dugme trenutno nije pritisnuto
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        //Brisanje ekrana
        glClearColor(0.5, 0.5, 0.5, 1.0); //Podesavanje boje pozadine: RGBA (R - Crvena, G - Zelena, B - Plava, A = neprovidno; Opseg od 0 do 1, gdje je 0 crno a 1 svijetlo)
        glClear(GL_COLOR_BUFFER_BIT);

        // [KOD ZA CRTANJE]
        glUseProgram(unifiedShader); //Izaberi novopeceni sejder program za crtanje i koristi ga za svo naknadno crtanje (Ukoliko ne aktiviramo neke druge sejder programe)
        glBindVertexArray(VAO); //Izaberemo sta zelimo da crtamo
        glDrawArrays(GL_TRIANGLES, 0, 3); //To i nacrtamo
        //glDrawArrays(tip primitive, indeks pocetnog tjemena, koliko narednih tjemena crtamo);

        //Zamjena vidljivog bafera sa pozadinskim
        glfwSwapBuffers(window);

        //Hvatanje dogadjaja koji se ticu okvira prozora (promjena velicine, pomjeranje itd)
        glfwPollEvents();
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);
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
