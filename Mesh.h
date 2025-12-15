#ifndef MESH_CLASS
#define MESH_CLASS

#include <iostream>
#include <random>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "t.h"

/*
struct Vertex {
	glm::vec3 Position;
};
*/


class Mesh : public t_package {
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
public:
	VAO vao;
	VBO vbo;
	EBO ebo;

	Mesh(std::vector<GLfloat>& meshvertices, std::vector<GLuint>& meshindices) :

		vertices(meshvertices),
		indices(meshindices),
		vao(),
		vbo(&vertices[0], vertices.size() * sizeof(GLfloat)),
		ebo(&indices[0], indices.size() * sizeof(GLuint))

	{

		vbo.Bind();
		ebo.Bind();

		vao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.LinkVBO(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		vao.Unbind();
		vbo.Unbind();


		ebo.Unbind();

		//rest

		SetMatrix( glm::mat4(1.0f) );
	};

	virtual void Render(Shader ShaderProgram) {
		glm::mat4 topass = GetMatrix();
		ShaderProgram.Activate();
		ShaderProgram.SetMat4("modl",  topass);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};


private:

};


#endif