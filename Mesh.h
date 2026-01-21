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

class Mesh {
	friend class Model;
protected:
public:
	t_package t;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	VAO vao;
	VBO vbo;
	EBO ebo;

	Mesh(std::vector<Vertex>& meshvertices, std::vector<GLuint>& meshindices/*, std::vector<Texture> textures*/) :
		vertices(meshvertices),
		indices(meshindices)
		//textures(textures)
	{
		NormalizeVertices();
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

	void SetVertices(std::vector<Vertex>& e) {
		glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(e), &e[0]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo.ID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(e), &e[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UpdateVertices() {
		glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(vertices), &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo.ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void NormalizeVertices() {
		float biggestx;
		float smallestx;
		float biggesty;
		float smallesty;
		float biggestz;
		float smallestz;
		bool initalized = false;

		for (Vertex& vertex : vertices) {
			if (not initalized) {
				biggestx = vertex.Position.x;
				smallestx = vertex.Position.x;
				biggesty = vertex.Position.y;
				smallesty = vertex.Position.y;
				biggestz = vertex.Position.z;
				smallestz = vertex.Position.z;

				initalized = true;
			}
			else {
				biggestx = std::max(biggestx, vertex.Position.x);
				smallestx = std::min(smallestx, vertex.Position.x);
				biggesty = std::max(biggesty, vertex.Position.y);
				smallesty = std::min(smallesty, vertex.Position.y);
				biggestz = std::max(biggestz, vertex.Position.z);
				smallestz = std::min(smallestz, vertex.Position.z);
			}
		}
		

		if (not initalized) {
			std::cout << "NO VERTICES";
		}
		else {
			t.ScaleTo(glm::vec3(biggestx - smallestx, biggesty - smallesty, biggestz - smallestz));
			float midx = (biggestx + smallestx) / 2.0f;
			float midy = (biggesty + smallesty) / 2.0f;
			float midz = (biggestz + smallestz) / 2.0f;
			float xf = biggestx - smallestx;
			float yf = biggesty - smallesty;
			float zf = biggestz - smallestz;

			for (Vertex& vertex : vertices) {
				vertex.Position = glm::vec3(
					(vertex.Position.x - midx) / xf,
					(vertex.Position.y - midy) / yf,
					(vertex.Position.z - midz) / zf
				);
			}
			UpdateVertices();
		}

	}

	virtual void Render(Shader& ShaderProgram, glm::mat4 modlmatrix) {
		glm::mat4 topass = modlmatrix * t.GetMatrix();

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
		ShaderProgram.SetMat4("modl", topass);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};

	virtual void Render(Shader& ShaderProgram) {
		glm::mat4 topass = t.GetMatrix();

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

	virtual void RenderWireframe(Shader& ShaderProgram) {
		glm::mat4 topass = t.GetMatrix();

		ShaderProgram.Activate();
		ShaderProgram.SetMat4("modl", topass);
		vao.Bind();
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};



private:

};


#endif