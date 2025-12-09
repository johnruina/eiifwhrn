
//OVERARCHING LIBRARIES
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
#include "TestObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int height = 720;
unsigned int width = 1280;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));
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


    //WINDOW INITIALIZATION
    GLFWwindow* window = glfwCreateWindow(width,height,"eiifwhrn",NULL,NULL);
    if (window == NULL) {

        glfwTerminate();
        throw -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    gladLoadGL();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);

    glfwSwapInterval(1);
    
    Shader ShaderProgram("default.vert", "default.frag");
    
    Pyramid testpyra({0.0f,0.0f,0.0f});
    
/*
    std::vector<Pyramid> pyramids = {};
    unsigned int amount = 100;
    for (int i = 0; i < amount; i++) {
        Pyramid testpyra({ float(rand() % 100) / 10.0f,float(rand() % 100) / 10.0f ,float(rand() % 100) / 10.0f });
        pyramids.push_back(testpyra);
    }
*/
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

    //Texture testtex("testtexture.png", GL_TEXTURE_2D, GL_TEXTURE0,GL_RGBA,GL_UNSIGNED_BYTE);

    //testtex.texUnit(ShaderProgram, "tex0", 0);

    //Material testmaterial(glm::vec3(1.0f), { 0.5f,0.5f,0.5f }, { 1.0f,1.0f,1.0f }, 0.5f);

    while (!glfwWindowShouldClose(window))
    {   

        ProcessInputs(window);

        camera.Matrix(90.0f, 0.05f, 200.0f, ShaderProgram);

        //RENDER SCENE

        //SKYBOX



        //SCENE

        glClearColor(212./255., 223. / 255., 232. / 255.,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderProgram.Activate();
        
        //mat

        //testmaterial.Bind(ShaderProgram);
        //testtex.Bind();
        //testtex.Unbind();
        testpyra.Render(ShaderProgram);
        /*
        for (Pyramid pyra : pyramids) {
            pyra.Render(ShaderProgram);
        }
        */
        //LEARN INSTANCING SOME OTHER DAY
        //glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>( 18 ), GL_UNSIGNED_INT, 5, amount);

        //RENDER GUI

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
    for (Pyramid pyra : pyramids) {
        pyra.~Pyramid();
    }
    */
    
    testpyra.~Pyramid();

    //testtex.Delete();

    ShaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}