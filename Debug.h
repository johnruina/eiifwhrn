
#ifndef DEBUG_CLASS
#define DEBUG_CLASS

#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Audio.h"
#include "Camera.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "texture.h"
#include "Material.h"
#include "Keyboard.h"
#include "Model.h"
#include "DirLight.h"
/*
Shader DebugCubeShader;
UINT DebugCubeVAO;
UINT DebugCubeEBO;
UINT DebugCubeVBO;
*/

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

std::vector<Vertex> CUBEVERTICES = {
{{0.5f,0.5f,0.5f}, {0.0f,1.0f,0.0f},{ 0.0f,0.0f} },
{{0.5f,0.5f,-0.5f}, {0.0f,1.0f,0.0f},{ 0.0f,1.0f}},
{{0.5f,-0.5f,-0.5f}, {0.0f,1.0f,0.0f},{ 1.0f,1.0f}},
{{0.5f,-0.5f,0.5f}, {0.0f,1.0f,0.0f}, {1.0f,0.0f}},

{{-0.5f,0.5f,0.5f}, {0.0f,-1.0f,0.0f}, {0.0f,0.0f}},
{{-0.5f,0.5f,-0.5f}, {0.0f,-1.0f,0.0f}, {0.0f,1.0f}},
{{-0.5f,-0.5f,-0.5f}, {0.0f,-1.0f,0.0f}, {1.0f,1.0f}},
{{-0.5f,-0.5f,0.5f},{ 0.0f,-1.0f,0.0f}, {1.0f,0.0f}},

{{0.5f,0.5f,0.5f}, {1.0f,0.0f,0.0f},{ 0.0f,0.0f} },
{{0.5f,0.5f,-0.5f}, {1.0f,0.0f,0.0f},{ 0.0f,1.0f}},
{{-0.5f,0.5f,-0.5f}, {1.0f,0.0f,0.0f},{ 1.0f,1.0f}},
{{-0.5f,0.5f,0.5f}, {1.0f,0.0f,0.0f}, {1.0f,0.0f}},

{{0.5f,-0.5f,0.5f}, {-1.0f,0.0f,0.0f}, {0.0f,0.0f}},
{{0.5f,-0.5f,-0.5f}, {-1.0f,0.0f,0.0f}, {0.0f,1.0f}},
{{-0.5f,-0.5f,-0.5f}, {-1.0f,0.0f,0.0f}, {1.0f,1.0f}},
{{-0.5f,-0.5f,0.5f},{ -1.0f,0.0f,0.0f}, {1.0f,0.0f}},

{{0.5f,0.5f,0.5f}, {0.0f,0.0f,1.0f},{ 0.0f,0.0f} },
{{-0.5f,0.5f,0.5f}, {0.0f,0.0f,1.0f},{ 0.0f,1.0f}},
{{-0.5f,-0.5f,0.5f}, {0.0f,0.0f,1.0f},{ 1.0f,1.0f}},
{{0.5f,-0.5f,0.5f}, {0.0f,0.0f,1.0f}, {1.0f,0.0f}},

{{0.5f,0.5f,-0.5f}, {0.0f,0.0f,-1.0f}, {0.0f,0.0f}},
{{-0.5f,0.5f,-0.5f}, {0.0f,0.0f,-1.0f}, {0.0f,1.0f}},
{{-0.5f,-0.5f,-0.5f}, {0.0f,0.0f,-1.0f}, {1.0f,1.0f}},
{{0.5f,-0.5f,-0.5f},{ 0.0f,0.0f,-1.0f}, {1.0f,0.0f}}
};

std::vector<unsigned int> CUBEINDICES = {
    0, 3, 1,  3, 2, 1,
    4, 5, 7,  5, 6, 7,

    8, 9, 11,  9, 10, 11,
    12, 15, 13,  15, 14, 13,

    16, 17, 19,  17, 18, 19,
    20, 23, 21,  23, 22, 21
};

void Output(float& to) {
    std::cout << to << '\n';
}

void Output(int& to) {
    std::cout << to << '\n';
}

void Output(glm::vec3& tooutput) {
	std::cout << tooutput.x << ' ' << tooutput.y << ' ' << tooutput.z << '\n';
}

Mesh* CreateCubeMesh() {
    return new Mesh(CUBEVERTICES, CUBEINDICES);
}

#endif