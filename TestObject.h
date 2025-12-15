
#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <random>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Mesh.h"

std::vector<GLfloat>  pyramidvertices = {
-0.5f, 0.0f,0.5f, 0.6f, 0.15f,0.123f, 0.0f,0.0f,
-0.5f,0.0f,-0.5f, 0.2f,0.13f,0.35f, 0.0f,1.0f,
0.5f,0.0f,-0.5f, 0.16f,0.3f,0.6f, 1.0f,1.0f,
0.5f,0.0f,0.5f, 0.2f,0.25f,0.2f, 1.0f,0.0f,
0.0f,0.8f,0.0f, 0.2f,0.25f,0.4f, 1.0f,0.0f,
};

std::vector<GLuint> pyramidindices= {
    2,1,0,
    3,2,0,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4,
};

class Pyramid  : public Mesh {

public:
    Pyramid() : Mesh(pyramidvertices, pyramidindices) {};
private:
};