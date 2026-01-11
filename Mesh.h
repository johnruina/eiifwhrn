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
#include "Vertex.h"
#include "texture.h"

class Mesh : public t_package {
protected:
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture>      textures;
	VAO vao;
	VBO vbo;
	EBO ebo;

	Mesh(std::vector<Vertex>& meshvertices, std::vector<GLuint>& meshindices/*, std::vector<Texture> textures*/) :
		vertices(meshvertices),
		indices(meshindices)
		//textures(textures)
	{
		vbo.BufferData(&vertices[0], vertices.size() * sizeof(Vertex));
		ebo.BufferData(&indices[0], indices.size() * sizeof(GLuint));

		vbo.Bind();
		ebo.Bind();

		vao.LinkVBO(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.LinkVBO(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		vao.LinkVBO(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();
	};


	void Clear() {
		vertices = std::vector<Vertex>();
		indices = std::vector<GLuint>();
	}

	virtual void Render(Shader ShaderProgram) {
		glm::mat4 topass = GetMatrix();

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to string
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(ShaderProgram.ID, (name + number).c_str()), i);
			// and finally bind the texture
			textures[i].Bind();
		}

		ShaderProgram.Activate();
		ShaderProgram.SetMat4("modl",  topass);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};



private:

};


#endif