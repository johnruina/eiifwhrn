#ifndef PARTICLE_CLASS
#define PARTICLE_CLASS

#include <random>
#include <vector>

#include<glm/glm.hpp>

#include "QuadVertices.h"
#include "t.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Vertex.h"


class Particle {
public:

	t_package t;

	Particle() {

	}

	void Render() {

	}

	void Step(float dt) {
		t.TranslateBy(linearvelocity*dt);
	}
	glm::vec3 linearvelocity;

private:
};

class ParticleEmitter {
public:

	t_package t;
	
	std::vector<Particle*> particles;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	VAO vao;
	VBO vbo;
	EBO ebo;

	ParticleEmitter() {
		vertices = quadVertices3D;
		indices = quadIndices3D;

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
	}

	void Emit() {
		Particle* np = new Particle();

		np->t.TranslateTo(t.GetTranslation());
		std::cout << t.GetFrontVector().x << ' ' << t.GetFrontVector().y << ' ' << t.GetFrontVector().z << '\n';
		np->linearvelocity = t.GetFrontVector() * float(rand() % 200) / 50.0f;
		//np->linearvelocity = np->linearvelocity * glm::quat({ glm::radians(float(rand() % 30)) - 15.0f,glm::radians(float(rand() % 30)) - 15.0f,glm::radians(float(rand() % 30)) - 15.0f });
		np->t.ScaleTo({0.2f,0.2f,0.0f});
		np->t.RotateToQuaternion(t.GetRotationQuaternion());
		

		particles.push_back(np);
	}

	void Step(float dt) {
		for (Particle* p : particles) {
			p->Step(dt);
		}
	}

	void Render(Shader ShaderProgram) {

		ShaderProgram.Activate();

		vao.Bind();

		for (Particle* p: particles) {
			ShaderProgram.SetMat4("modl", p->t.GetMatrix());
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		vao.Unbind();
	}

private:

};

#endif