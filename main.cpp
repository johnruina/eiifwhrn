
//OVERARCHING LIBRARIES
#include <iostream>
#include <vector>
#include <random>

//OPENGL LIBRARIES
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//MANMADE LIBRARIES
#include "Camera.h"
#include "shaderClass.h"
#include "Window.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "texture.h"
#include "Material.h"
#include "Keyboard.h"
#include "Cube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int height = 720;
unsigned int width = 1280;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
bool firstmouse = false;

Keyboard keyboard;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyboard.KeyDown(key);

    }
    else if (action == GLFW_RELEASE) {
        keyboard.KeyUp(key);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstmouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstmouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top


    camera.ProcessMouseMovement(xoffset, yoffset);
    glfwSetCursorPos(window,width/2,height/2);

    //lastX = xpos;
    //lastY = ypos;
}

void ProcessInputs(GLFWwindow* window) {
    
    if (keyboard.IsKeyDown(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (keyboard.IsKeyDown('W')) {
        camera.position += camera.speed * camera.Front;
    }
    if (keyboard.IsKeyDown('S')) {
        camera.position -= camera.speed * camera.Front;
    }
    if (keyboard.IsKeyDown('A')) {
        camera.position -= camera.speed * camera.Right;
    }
    if (keyboard.IsKeyDown('D')) {
        camera.position += camera.speed * camera.Right;
    }
    if (keyboard.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        camera.speed *= 1.005;
    }
    else {
        camera.speed = camera.originalspeed;
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); 

    //WINDOW INITIALIZATION
    GLFWwindow* window = glfwCreateWindow(width, height, "eiifwhrn", NULL, NULL);
    if (window == NULL) {

        glfwTerminate();
        throw - 1;
    }


    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);
    glViewport(0, 0, width, height);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    Shader ShaderProgram("default.vert", "default.frag");
    
    std::vector<Cube> cubes;
    cubes.push_back(Cube());

    /*
    std::vector<std::string> faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    */

    //TEXTURE

    Texture testtex("testtexture.png", GL_TEXTURE_2D, GL_TEXTURE0,GL_RGB,GL_UNSIGNED_BYTE);

    testtex.texUnit(ShaderProgram, "tex0", 0);

    Material testmaterial(glm::vec3(1.0f), { 0.5f,0.5f,0.5f }, { 1.0f,1.0f,1.0f }, 0.5f);

    unsigned long int frame = 0;
    unsigned long prev = 0;

    while (!glfwWindowShouldClose(window))
    {   
        //START OF FRAME
        frame++;

        //INPUTS
        ProcessInputs(window);

        //WORLD STUFF
        unsigned int onceeveryframes = 1;
        unsigned int timestogen = 1;
        if (frame % onceeveryframes == 0) {

            for (int i = 0; i < timestogen; i++) {
             

                if (cubes.size() > 2000) {
                    cubes.erase(cubes.begin());
                }

                unsigned int whichdir = rand() % 5;

                if (whichdir >= prev) {
                    whichdir++;
                }

                glm::vec3 poschange(0.0f);
                switch (whichdir) {
                case 0:
                    poschange.x = 2.0f;
                    break;
                case 1:
                    poschange.x = -2.0f;
                    break;
                case 2:
                    poschange.y = 2.0f;
                    break;
                case 3:
                    poschange.y = -2.0f;
                    break;
                case 4:
                    poschange.z = 2.0f;
                    break;
                case 5:
                    poschange.z = -2.0f;
                    break;
                }

                prev = whichdir;
                Cube newcube;
                newcube.TranslateBy(cubes[cubes.size() - 1].GetPosition() + poschange);
                cubes.push_back(newcube);

            }
        }

        for (Cube cu : cubes) {
            
        }
        
        //RENDER SCENE

        camera.Matrix(90.0f, 0.05f, 200.0f, ShaderProgram);

        //SKYBOX



        //SCENE

        glClearColor(212./255., 223. / 255., 232. / 255.,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderProgram.Activate();
        
        //mat

        testmaterial.Bind(ShaderProgram);
        testtex.Bind();
        //testtex.Unbind();

        for (Cube cube : cubes) {
            cube.Render(ShaderProgram);
        }
        
        //LEARN INSTANCING SOME OTHER DAY
        //glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>( 18 ), GL_UNSIGNED_INT, 5, amount);

        //RENDER GUI

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //testtex.Delete();

    ShaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}