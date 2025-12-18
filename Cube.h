#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H


#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"

static std::vector<Vertex> cubevertices = {
	{{1.0f,1.0f,-1.0f}, {0.6f, 0.15f,0.123f}, {0.0f,0.0f}},
	{{-1.0f,1.0f,-1.0f}, {0.2f,0.13f,0.35f}, {0.0f,1.0f} },
{{-1.0f,1.0f,1.0f}, {0.16f,0.3f,0.6f}, {1.0f,1.0f} },
	{{1.0f,1.0f,1.0f}, {0.2f,0.25f,0.2f}, {1.0f,0.0f }},
{{1.0f,-1.0f,-1.0f}, {0.6f, 0.15f,0.123f}, {0.0f,0.0f} },
{{-1.0f,-1.0f,-1.0f},{ 0.2f,0.13f,0.35f}, {0.0f,1.0f}},
{{-1.0f,-1.0f,1.0f}, {0.16f,0.3f,0.6f}, {1.0f,1.0f} },
{{1.0f,-1.0f,1.0f}, {0.2f,0.25f,0.2f}, {1.0f,0.0f} },
};

static std::vector<GLuint> cubeindices = {
1,0,3,
1,3,2,
0,4,7,
0,7,3,
3,7,6,
3,6,2,
2,6,5,
2,5,1,
1,5,4,
1,4,0,
5,6,7,
4,5,7
};

class Cube : public Mesh {

public:

	Cube() : Mesh(cubevertices, cubeindices) {};


private:

};


#endif