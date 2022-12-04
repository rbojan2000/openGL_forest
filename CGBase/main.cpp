#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "shader.hpp"
#include "model.hpp" //Klasa za ucitavanje modela
#include "renderable.hpp" //Klasa za bafere


const int WindowWidth = 800;
const int WindowHeight = 800;
const std::string WindowTitle = "GIM Flight Sim";
const float TargetFPS = 60.0f;
const float TargetFrameTime = 1.0f / TargetFPS;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) //Aktivira se pri mijenjanju velicine prozora
{
    glViewport(0, 0, width, height);
    /*glViewport(X, Y, w, h) mapira prikaz scene na dio prozora. Vrijednosti parametara su pozitivni cijeli brojevi
    * (radi sa rezolucijom ekrana, ne sa NDC sistemom)
    * X, Y = koordinate donjeg lijevog coska (0,0 je donji lijevi cosak prozora)
    * width, height = duzina i visina prikaza
     */
}

static void
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    bool IsDown = action == GLFW_PRESS || action == GLFW_REPEAT;
    switch (key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    }
}

static void
ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    GLFWwindow* Window = 0;
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(ErrorCallback);

    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(), 0, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetKeyCallback(Window, KeyCallback);
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init glew: " << glewGetErrorString(GlewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader Basic("shaders/basic.vert", "shaders/basic.frag");

    //Ucitavanje modela
    Model Jap("ki61/13564_Golden_Lion_Tamarin_v1_L2.obj");
    if (!Jap.Load())
    {
        std::cout << "Failed to load model!\n";
        glfwTerminate();
        return -1;
    }

    // Vertices coordinates
    GLfloat valleyVertices[] =
    { //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
        -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
         1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
         1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
    };

    GLuint valleyIndices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    float cubeVertices[] = //Bijela kocka. Boju cemo mijenjati sa uCol
    {
        -0.2, -0.2, -0.2,       0.0, 0.0, 0.0,
        +0.2, -0.2, -0.2,       0.0, 0.0, 0.0,
        -0.2, -0.2, +0.2,       0.0, 0.0, 0.0,
        +0.2, -0.2, +0.2,       0.0, 0.0, 0.0,

        -0.2, +0.2, -0.2,       0.0, 0.0, 0.0,
        +0.2, +0.2, -0.2,       0.0, 0.0, 0.0,
        -0.2, +0.2, +0.2,       0.0, 0.0, 0.0,
        +0.2, +0.2, +0.2,       0.0, 0.0, 0.0,
    };


    float pyramideVertices[] =
    {
        -0.2, +0.0, +0.2,     0.0, 0.0, 0.0,
        -0.2, +0.0, -0.2,     0.0, 0.0, 0.0,
        +0.2, +0.0, -0.2,     0.0, 0.0, 0.0,
        +0.2, +0.0, +0.2,     0.0, 0.0, 0.0,
        +0.0, +0.4, +0.0,     0.0, 0.0, 0.0,
    };


    float funkyCubeVertices[] = //Sarena kocka
    {
        -0.4, -0.4, -0.2,       0.0, 0.0, 0.0,
        +0.4, -0.4, -0.2,       0.0, 0.0, 0.0,
        -0.4, -0.4, +0.2,       0.0, 0.0, 0.0,
        +0.4, -0.4, +0.2,       0.0, 0.0, 0.0,

        -0.4, +0.4, -0.2,       0.0, 0.0, 0.0,
        +0.4, +0.4, -0.2,       0.0, 0.0, 0.0,
        -0.4, +0.4, +0.2,       0.0, 0.0, 0.0,
        +0.4, +0.4, +0.2,       0.0, 0.0, 0.0,
    };

    unsigned int cubeIndices[] = {
        0, 1, 3,
        0, 2, 3,

        4, 6, 7,
        4, 5, 7,

        3, 6, 2,
        3, 6, 7,

        0, 4, 1,
        1, 4, 5,

        0, 6, 2,
        0, 4, 6,

        1, 3, 7,
        1, 7, 5
    }; //Indeksi za formiranje kocke

    unsigned int pyramidIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };


    //Pravljenje Renderable objekta (Generise VAO i VBO pri konstrukciji)
    Renderable cube(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));
    Renderable funkyCube(funkyCubeVertices, sizeof(funkyCubeVertices), cubeIndices, sizeof(cubeIndices));
    Renderable piramid(pyramideVertices, sizeof(pyramideVertices), pyramidIndices, sizeof(pyramidIndices));
    Renderable valley(valleyVertices, sizeof(valleyVertices), valleyIndices, sizeof(valleyIndices));


    //Matrica modela (transformacije)
    glm::mat4 m(1.0f);
    //mat4(A) generise matricu 4x4 sa A na glavnoj dijagonali

    //Matrica pogleda (kamere)
    glm::mat4 v = glm::lookAt(glm::vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //lookAt(pozicija kamere, tacka u koju kamera gleda, kako je kamera rotirana po osi definisanoj sa prethodne dvije tacke)
    
    //Matrica projekcije
    glm::mat4 p = glm::perspective(glm::radians(100.0f), (float)WindowWidth / WindowHeight, 0.1f, 100.0f);
    //perspective(FOV ugao, aspect ratio prozora, prednja odsjecna ravan i zadnja odsjecna ravan)
    
    float angle = 0;//Ugao za rotiranje aviona
    float movement = 0;//Offset za pomijeranje aviona

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.3, 1.0, 1.0);


    float FrameStartTime = glfwGetTime();
    float FrameEndTime = glfwGetTime();
    float dt = FrameEndTime - FrameStartTime;
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        FrameStartTime = glfwGetTime();
        glUseProgram(Basic.GetId());


        Basic.SetProjection(p);
        Basic.SetView(v);
                
       

        //Crtanje sunca
        Basic.SetColor(1, 0.5, 0);
        m = glm::scale(glm::mat4(4.0f), glm::vec3(abs(sin(glfwGetTime() / 2)) / 18 + 0.5));
        m = glm::translate(m, glm::vec3(-3.2, 4.5, +0.5)); //Pomjeranje za XYZ (cetvrta kolona matrice)
        m = glm::rotate(m, glm::radians(30.0f), glm::vec3(1.0, 1.0, 1.0));
        Basic.SetModel(m);
        cube.Render();
        m = glm::rotate(m, glm::radians(100.0f), glm::vec3(1.0, 1.0, 1.0));
        Basic.SetModel(m);
        Basic.SetColor(1, 0.2 + abs(sin(glfwGetTime())), 0);
        cube.Render();

        //Crtanje majmuna
        Basic.SetColor(0.5, 0.4, 0.1);
        m = glm::scale(glm::mat4(1.0f) , glm::vec3(0.02, 0.02, 0.02)); //Skaliranje objekta po XYZ (dijagonala matrice. Ne stavljati 0!)
        m = glm::translate(m, glm::vec3(100.5, 35.5, 0.1)); //Pomjeranje za XYZ (cetvrta kolona matrice)
        m = glm::rotate(m, glm::radians(240.0f), glm::vec3(1.0, 1.0, 1.0));
        
        Basic.SetModel(m);
        Jap.Render();

        

        //Crtanje drveca
        Basic.SetColor(0, 1, 0);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, 0.193, 0.1));
        Basic.SetModel(m);
        funkyCube.Render();

        Basic.SetColor(0.58, 0.29, 0);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, -0.5, 0.1));
        m = glm::rotate(m, glm::radians(120.0f), glm::vec3(1.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(2.5, 1.0, 1.0));
        Basic.SetModel(m);
        cube.Render();


        Basic.SetColor(0.228, 0.208, 0.10);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, 0.193, 0.1));
        Basic.SetModel(m);
        funkyCube.Render();

        Basic.SetColor(0, 0.0, 0);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, -0.5, 0.1));
        m = glm::rotate(m, glm::radians(120.0f), glm::vec3(1.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(2.5, 1.0, 1.0));
        Basic.SetModel(m);
        cube.Render();




        //Crtanje piramide
        Basic.SetColor(0.6, 0.6, 0.6);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, -0.3, 5.0));
        m = glm::rotate(m, glm::radians(0.0f), glm::vec3(0.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(20.0, 20.0, -2.0));
        Basic.SetModel(m);
        piramid.Render();

        Basic.SetColor(0.6, 0.6, 0.6);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(-7.5, -0.3, 5.0));
        m = glm::rotate(m, glm::radians(0.0f), glm::vec3(0.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(20.0, 20.0, -2.0));
        Basic.SetModel(m);
        piramid.Render();

        
        Basic.SetColor(0.2, 0.2, 0.2);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(2.5, -0.3, 7.0));
        m = glm::rotate(m, glm::radians(0.0f), glm::vec3(0.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(20.0, 20.0, -2.0));
        Basic.SetModel(m);
        piramid.Render();
        
        Basic.SetColor(0.2, 0.2, 0.2);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5, -0.3, 7.0));
        m = glm::rotate(m, glm::radians(0.0f), glm::vec3(0.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(20.0, 20.0, -2.0));
        Basic.SetModel(m);
        piramid.Render();


        //crtanje doline
        Basic.SetColor(0.31, 0.8, 0.31);
        m = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5, -0.3, 7.0));
        //m = glm::rotate(m, glm::radians(0.0f), glm::vec3(0.0, 1.0, 1.0));
        m = glm::scale(m, glm::vec3(35.0, 20.0, -2.0));
        Basic.SetModel(m);
        valley.Render();

        glUseProgram(0);
        glfwSwapBuffers(Window);

        FrameEndTime = glfwGetTime();
        dt = FrameEndTime - FrameStartTime;
        if (dt < TargetFPS) {
            int DeltaMS = (int)((TargetFrameTime - dt) * 1e3f);
            std::this_thread::sleep_for(std::chrono::milliseconds(DeltaMS));
            FrameEndTime = glfwGetTime();
        }
        dt = FrameEndTime - FrameStartTime;
    }

    glfwTerminate();
    return 0;
}



