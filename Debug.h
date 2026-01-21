
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
    0,1,3, 1,2,3,
    4,5,7, 5,6,7,
    
    8,9,11, 9,10,11,
    12,13,15, 13,14,15,

    16,17,19, 17,18,19,
    20,21,23, 21,22,23,
};
/*
void InitializeDebug() {
	DebugCubeShader = Shader("DebugCube.vert", "DebugCube.frag");
    unsigned int DebugCubeVBO, DebugCubeVAO, DebugCubeEBO;
    glGenVertexArrays(1, &DebugCubeVAO);
    glGenBuffers(1, &DebugCubeVBO);
    glGenBuffers(1, &DebugCubeEBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(DebugCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, DebugCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, CUBEVERTICES.size(), &CUBEVERTICES[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DebugCubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(&CUBEINDICES[0]), &CUBEINDICES[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
*/

void RenderDebugBox(t_package t) {
    
    
}

void OutputVec3(glm::vec3 tooutput) {
	std::cout << tooutput.x << ' ' << tooutput.y << ' ' << tooutput.z << '\n';
}

Mesh CreateCubeMesh() {
    return Mesh(CUBEVERTICES, CUBEINDICES);
}

#endif