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

#include "RenderSystem.h"
#include "Object.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "t.h"
#include "tFunctions.h"
#include "Vertex.h"
#include "texture.h"

class Mesh : public Object , public Renderable {
	friend class Model;
protected:
public:
	t_package t;
	p_package p;
	Physics* bindedphysicsengine;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	VAO vao;
	VBO vbo;
	EBO ebo;

	//DO NOT MAKE A MESH WITH A ZERO IN SCALE PRETTY PLEASE OR ELSE ITLL FRY A BUNCHA MATH AND FEATURES

	Mesh() {

	};

	Mesh(const std::vector<Vertex>& meshvertices, const std::vector<GLuint>& meshindices/*, std::vector<Texture> textures*/) :
		vertices(meshvertices),
		indices(meshindices)
		//textures(textures)
	{
		InitializeMesh(meshvertices,meshindices);
	};

	void InitializeMesh(const std::vector<Vertex>& meshvertices, const std::vector<GLuint>& meshindices) {
		vertices = meshvertices;
		indices = meshindices;
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

		NormalizeVertices();
		UpdateVertices();
	}

	Mesh(const Mesh& other) {

		this->vertices = other.vertices;
		this->indices = other.indices;
		this->textures = other.textures;
		this->t = other.t;

		this->vbo = VBO();
		this->vao = VAO();
		this->ebo = EBO();

		this->vbo.BufferData(&vertices[0], vertices.size() * sizeof(Vertex));
		this->ebo.BufferData(&indices[0], indices.size() * sizeof(GLuint));

		this->vbo.Bind();
		this->ebo.Bind();

		this->vao.LinkVBO(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		this->vao.LinkVBO(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		this->vao.LinkVBO(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		this->vao.Unbind();
		this->vbo.Unbind();
		this->ebo.Unbind();
	}

	virtual ~Mesh() {
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
		//delete this;
	}

	void InitializePhysics() {
		p.t = &t;
	}

	void AddPhysicsToEngine(Physics& engine) {
		engine.AddObject(&p);
		bindedphysicsengine = &engine;
	}

	void RemovePhysicsFromEngine() {
		if (bindedphysicsengine != nullptr) {
			bindedphysicsengine->RemoveObject(&p);
			bindedphysicsengine = nullptr;
		}
	}

	float GetVolume() const {
		float runningtotal = 0.0f;

		for (int i = 0; i < indices.size() / 3; i++) {
			runningtotal += VolumeOfTriangle(vertices[indices[i * 3]].Position, vertices[indices[i * 3 + 1]].Position, vertices[indices[i * 3 + 2]].Position);
		}
		
		return runningtotal * t.GetScale().x * t.GetScale().y * t.GetScale().z;
	}

	void Slice(glm::vec3 origin, glm::quat slicequat, std::vector<Mesh*>& meshvectorforthestufftogoin) {
		
		glm::vec3 pos = t.GetTranslation() + (glm::conjugate(slicequat) * (origin - t.GetTranslation()));;

		std::vector<glm::vec3> newvs = {};

		bool posfound = false;
		bool negfound = false;
		for (int i = 0; i < vertices.size(); i++) {
			glm::vec3 newpos = glm::conjugate(slicequat) * vertices[i].Position;
			newvs.push_back(newpos);
			if (newpos.y + pos.y > 0) {
				posfound = true;
			}
			else {
				negfound = true;
			}
		}

		if (posfound and negfound) {

			
			Mesh* posmesh = new Mesh(*this);
			Mesh* negmesh = new Mesh(*this);
			posmesh->indices = std::vector<GLuint>();
			negmesh->indices = std::vector<GLuint>();

			for (int i = 0; i < indices.size() / 3; i++) {
				if (newvs[indices[i * 3]].y > 0 and newvs[indices[i * 3 + 1]].y > 0 and newvs[indices[i * 3 + 2]].y > 0) {
					posmesh->indices.push_back(indices[i * 3]);
					posmesh->indices.push_back(indices[i * 3 + 1]);
					posmesh->indices.push_back(indices[i * 3 + 2]);
				}
				else if (newvs[indices[i * 3]].y <= 0 and newvs[indices[i * 3 + 1]].y <= 0 and newvs[indices[i * 3 + 2]].y <= 0) {
					negmesh->indices.push_back(indices[i * 3]);
					negmesh->indices.push_back(indices[i * 3 + 1]);
					negmesh->indices.push_back(indices[i * 3 + 2]);
				}
				else {
					//wallahi...

				}

			}

			posmesh->NormalizeVertices();
			negmesh->NormalizeVertices();
			meshvectorforthestufftogoin.reserve(meshvectorforthestufftogoin.size() + 2);
			meshvectorforthestufftogoin.push_back(posmesh);

			meshvectorforthestufftogoin.push_back(negmesh);
			
		}
		
	}

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

	void UpdateIndices() {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, NULL, sizeof(indices), &indices[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void NormalizeVertices() {
		float biggestx = FLT_MIN;
		float smallestx = FLT_MAX;
		float biggesty = FLT_MIN;
		float smallesty = FLT_MAX;
		float biggestz = FLT_MIN;
		float smallestz = FLT_MAX;
		for (Vertex& vertex : vertices) {

			biggestx = std::max(biggestx, vertex.Position.x);
			smallestx = std::min(smallestx, vertex.Position.x);
			biggesty = std::max(biggesty, vertex.Position.y);
			smallesty = std::min(smallesty, vertex.Position.y);
			biggestz = std::max(biggestz, vertex.Position.z);
			smallestz = std::min(smallestz, vertex.Position.z);
			
		}

		t.ScaleBy(glm::vec3(biggestx - smallestx, biggesty - smallesty, biggestz - smallestz));

		float midx = (biggestx + smallestx) / 2.0f;
		float midy = (biggesty + smallesty) / 2.0f;
		float midz = (biggestz + smallestz) / 2.0f;
		float xf = biggestx - smallestx;
		float yf = biggesty - smallesty;
		float zf = biggestz - smallestz;
		t.TranslateTo({midx,midy,midz});
		for (Vertex& vertex : vertices) {
			vertex.Position = glm::vec3(
				(vertex.Position.x - midx) / xf,
				(vertex.Position.y - midy) / yf,
				(vertex.Position.z - midz) / zf
			);
		}
	}

	virtual void Render(Shader& ShaderProgram, glm::mat4 modlmatrix) {
		glm::mat4 topass = modlmatrix * t.GetMatrix();

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		/*=
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
		*/
		ShaderProgram.Activate();
		ShaderProgram.SetMat4("modl", topass);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};

	void Render(Shader& ShaderProgram) override {
		glm::mat4 topass = t.GetMatrix();
		/*
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
		*/
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

std::optional<std::vector<glm::vec3>> RayIntersectsMesh(const Ray& ray, Mesh& mesh)
{
	//USE THIS AFTER A CHEAPER CHECK
	std::vector<glm::vec3> intersections = {};

	glm::mat4 inverse = glm::inverse(mesh.t.GetMatrix());
	glm::vec3 ray_origin = glm::vec3(inverse * glm::vec4(ray.origin, 1.0f));
	glm::vec3 ray_direction = glm::vec3(inverse * glm::vec4(ray.direction, 0.0f));

	for (int i = 0; i < mesh.indices.size() / 3; i++) {
		std::optional<glm::vec3> intersection = RayIntersectsTriangle({ ray_origin,ray_direction }, { mesh.vertices[mesh.indices[i * 3]].Position, mesh.vertices[mesh.indices[i * 3 + 1]].Position,mesh.vertices[mesh.indices[i * 3 + 2]].Position });
		if (intersection.has_value()) {
			intersections.push_back(intersection.value());
		}
	}
	
	if (intersections.size() > 0) return intersections;
	else return {};
}

void DivideFace() {

}

#endif