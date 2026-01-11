
//OVERARCHING LIBRARIES
#include <iostream>
#include <vector>
#include <random>

//OPENGL LIBRARIES
#include<glad/glad.h>
#include<GLFW/glfw3.h>


//math libraries
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

//MANMADE LIBRARIES
#include "Camera.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "FBO.h"
#include "texture.h"
#include "Material.h"
#include "Keyboard.h"
#include "Model.h"
#include "DirLight.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int height = 720;
unsigned int width = 1280;
float lastX = width / 2.0f;
float lastY = height / 2.0f;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
bool firstmouse = false;

Keyboard keyboard;

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
    glfwSetCursorPos(window, width / 2, height / 2);

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

    //SHADERS
    Shader ShaderProgram("default.vert", "default.frag");
    Shader ScreenShader("ScreenShader.vert", "ScreenShader.frag");
    
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //LIGHTING

    DirLight MainDirLight(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.0f, -1.0f, 1.0f));

    //

    std::vector<Model> cubes;
    cubes.push_back(Model("cratelookingthing.obj"));

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

    Model testobj("gun2.obj");
    Model terrain("terrain.obj");

    //TEXTURE

    Texture testtex("testtexture.png", "texture_diffuse");

    testtex.texUnit(ShaderProgram, "texture_diffuse1", 0);

    Texture metaltex("metaltex.png", "texture_diffuse");

    metaltex.texUnit(ShaderProgram, "texture_diffuse1", 0);

    //FRAMEBUFFER
    FBO framebuffer;
    framebuffer.Bind();
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    framebuffer.Unbind();


    //game logic and shit

    unsigned long int frame = 0;
    unsigned long prev = 0;

    //DEBUG/SHORTSTAY STUFF ONLY


    //LOOP
    while (!glfwWindowShouldClose(window))
    {
        //START OF FRAME
        frame++;

        //INPUTS
        ProcessInputs(window);

        //GAME LOGIC
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
                Model newcube("cratelookingthing.obj");
                newcube.TranslateBy(glm::vec3(float(rand()%1000)/10.0f, float(rand() % 1000) / 10.0f, float(rand() % 1000) / 10.0f));
                newcube.ScaleTo(4.5f);
                newcube.RotateToQuaternion(cubes[cubes.size() - 1].GetRotation());
                cubes.push_back(newcube);

            }
        }
        
        glm::vec3 np = camera.position + camera.Front * 0.75f + camera.Right * 0.5f + camera.Up * 0.5f;
        testobj.TranslateTo(np);

        for (Model& e : cubes) {

            if (frame % 90 < 30) {
                e.RotateByEulerAngles(glm::vec3(0,glm::radians(3.0f), 0));
            }
            else {
                e.RotateByEulerAngles(glm::vec3(0, 0, glm::radians(6.0f)));
            }
            
        }

        //RENDER SCENE

        //BIND FRAMEBUFFER
        
        framebuffer.Bind();

        glEnable(GL_DEPTH_TEST);

        //IMPORTANT
        glClearColor(212. / 255., 223. / 255., 232. / 255., 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //SKYBOX



        //STUFF

        ShaderProgram.Activate();

        camera.Matrix(90.0f, 0.05f, 200.0f, ShaderProgram);

        //LIGHTING STUFF

        MainDirLight.Bind(ShaderProgram);

        //SCENE


        //mat


        //testtex.Unbind();

        terrain.Render(ShaderProgram);

        testobj.Render(ShaderProgram);

        for (Model cube : cubes) {
            cube.Render(ShaderProgram);
        }


        //RENDER GUI

        //unbind stuff
        glBindVertexArray(0);

        // second pass
        
        framebuffer.Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f,0.0f,0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ScreenShader.Activate();
        
        ScreenShader.Set1F("colorContrast", sin((float)frame/15) * 0.5f + 0.5f);
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //end

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //testtex.Delete();

    ShaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}