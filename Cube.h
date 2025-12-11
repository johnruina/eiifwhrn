
#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"

GLfloat cubevertices[] = {
1.0f,1.0f,-1.0f, 0.6f, 0.15f,0.123f, 0.0f,0.0f,
-1.0f,1.0f,-1.0f, 0.2f,0.13f,0.35f, 0.0f,1.0f,
-1.0f,1.0f,1.0f, 0.16f,0.3f,0.6f, 1.0f,1.0f,
1.0f,1.0f,1.0f, 0.2f,0.25f,0.2f, 1.0f,0.0f,
1.0f,-1.0f,-1.0f, 0.6f, 0.15f,0.123f, 0.0f,0.0f,
-1.0f,-1.0f,-1.0f, 0.2f,0.13f,0.35f, 0.0f,1.0f,
-1.0f,-1.0f,1.0f, 0.16f,0.3f,0.6f, 1.0f,1.0f,
1.0f,-1.0f,1.0f, 0.2f,0.25f,0.2f, 1.0f,0.0f,
};

GLuint cubeindices[] = {
0,1,2,
0,3,2,
4,5,6,
4,7,6,
7,3,2,
7,6,2,
6,5,1,
6,2,1,
5,1,0,
5,4,0,
4,7,3,
4,0,3
};

class Cube : public Mesh {

public:

    VAO vao;
    VBO vbo;
    EBO ebo;

	Cube() : vao(), vbo(cubevertices, sizeof(cubevertices)),
        ebo(cubeindices, sizeof(cubeindices)) {

        //modelstuff
        vao.Bind();

        VBO vbo(cubevertices, sizeof(cubevertices));

        EBO ebo(cubeindices, sizeof(cubeindices));

        vbo.Bind();
        ebo.Bind();

        vao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        vao.LinkVBO(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        vao.Unbind();
        vbo.Unbind();


        ebo.Unbind();

        //rest

        t = glm::mat4(1.0f);
	}

	void Render(Shader& ShaderProgram) {
        //t = glm::rotate(t, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ShaderProgram.SetMat4("modl", t);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        vao.Unbind();
	}
private:

};


#endif