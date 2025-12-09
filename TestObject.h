
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

GLfloat vertices[] = {
-0.5f, 0.0f,0.5f, 0.6f, 0.15f,0.123f, 0.0f,0.0f,
-0.5f,0.0f,-0.5f, 0.2f,0.13f,0.35f, 0.0f,1.0f,
0.5f,0.0f,-0.5f, 0.16f,0.3f,0.6f, 1.0f,1.0f,
0.5f,0.0f,0.5f, 0.2f,0.25f,0.2f, 1.0f,0.0f,
0.0f,0.8f,0.0f, 0.2f,0.25f,0.4f, 1.0f,0.0f,
};

GLuint indices[] = {
    2,1,0,
    3,2,0,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4,
};

class Pyramid {

public:
    VAO vao;
    VBO vbo;
    EBO ebo;

    Pyramid(glm::vec3 pos) : vao(), vbo(vertices, sizeof(vertices)),
    ebo(indices, sizeof(indices))
    {
        vao.Bind();

        vao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        vao.LinkVBO(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));


        vao.Unbind();
        vbo.Unbind();


        ebo.Unbind();

        t = glm::mat4(1.0f);
        t = glm::translate(t,pos);
        t = glm::scale(t, {(rand()%5 + 1.0f) / 5.0f, (rand() % 5 + 1.0f)/5.0f ,float(rand() % 5 + 1.0f) / 5.0f });
    }

    ~Pyramid() {
        vao.Delete();
        vbo.Delete();
        ebo.Delete();
    }

	void Render(Shader& ShaderProgram) {
        t = glm::rotate(t, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ShaderProgram.SetMat4("modl", t);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
	}
private:
    glm::mat4 t;
};