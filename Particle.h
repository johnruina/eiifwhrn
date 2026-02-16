#ifndef PARTICLE_CLASS
#define PARTICLE_CLASS

#include <random>
#include <vector>
#include <map>

#include<glm/glm.hpp>

#include "Mesh.h"
#include "QuadVertices.h"
#include "t.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Vertex.h"
#include "texture.h"
#include "Camera.h"

class Particle {
public:

	t_package t;

	Particle() {

	}

	void Render() {

	}

	void Step(float dt) {
		t.RotateByEulerAnglesCumulate(angularvelocity * dt);
		t.TranslateBy(linearvelocity*dt);
	}
	glm::vec3 linearvelocity;
	glm::vec3 angularvelocity;
	float lifespan;
private:
};

class ParticleEmitter {
public:

	enum EmitDirection {
		Perpendicular,
		Aligned,
		Outward
	};

	EmitDirection emitdirection = Outward;
	//PARTICLE SETTINGS
	glm::vec3 size = {1.0f,1.0f,1.0f};
	glm::vec3 emitangle = {0.0f,0.0f,0.0f};
	float lifespan = 5.0f;
	float speed = 1.0f;
	glm::vec3 angularvelocity = {0.0f,0.0f,0.0f};
	bool facecamera = false;

	glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};

public:

	t_package t;
	
	std::vector<Particle*> particles;

	Texture* tex;

	Mesh mesh;
	
	ParticleEmitter() {
		mesh.InitializeMesh(quadVertices3D, quadIndices3D);
	}

	ParticleEmitter(const std::vector<Vertex>& meshvertices, const std::vector<GLuint>& meshindices) {
		mesh.InitializeMesh(meshvertices, meshindices);
	}

	void Emit() {
		Particle* np = new Particle();
		glm::quat rot = glm::quat(glm::vec3({ (float)rand() / (float)RAND_MAX * emitangle[0] - emitangle[0] / 2.0f,(float)rand() / (float)RAND_MAX * emitangle[1] - emitangle[1] / 2.0f,(float)rand() / RAND_MAX * emitangle[2] - emitangle[2] / 2.0f }));
		np->t.TranslateTo(t.GetTranslation());
		np->linearvelocity = (t.GetFrontVector() * speed) * rot;	
		np->angularvelocity = angularvelocity;
		np->t.ScaleTo(size);

		if (emitdirection == Perpendicular) {
			np->t.RotateToQuaternion(glm::conjugate(rot) * glm::quat({ glm::radians(90.0f),0.0f,0.0f }) * t.GetRotationQuaternion());
		}
		else if (emitdirection == Outward) {
			np->t.RotateToQuaternion(glm::conjugate(rot) * t.GetRotationQuaternion());
		}
		else {
			np->t.RotateToQuaternion(t.GetRotationQuaternion());
		}

		np->lifespan = lifespan;

		particles.push_back(np);
	}

	void Step(float dt) {
		for (auto it = particles.begin(); it != particles.end(); ) {
			Particle* p = *it;
			p->lifespan -= dt;

			if (p->lifespan <= 0.0f) {
				delete p;
				it = particles.erase(it);
			}
			else {
				p->Step(dt);
				++it;
			}
		}
	}

	void Render(Shader ShaderProgram, Camera& camera) {

		ShaderProgram.Activate();

		mesh.vao.Bind();

		ShaderProgram.Set4F("color",color);

		glActiveTexture(0);

		if (tex != nullptr) {
			glBindTexture(GL_TEXTURE_2D, tex->ID);
			ShaderProgram.SetInt("tex", 0);
		}
		else glBindTexture(GL_TEXTURE_2D, 0);

		std::map<float, Particle*> sorted;
		for (unsigned int i = 0; i < particles.size(); i++)
		{
			float distance = glm::length(camera.t.GetTranslation() - particles[i]->t.GetTranslation());
			sorted[distance] = particles[i];
		}

		for (std::map<float, Particle*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			if (facecamera) {
				ShaderProgram.SetMat4("modl", it->second->t.GetTranslationMatrix() * camera.t.GetRotationMatrix() * it->second->t.GetScaleMatrix());
			}
			else {
				ShaderProgram.SetMat4("modl", it->second->t.GetMatrix());
			}
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
		mesh.vao.Unbind();
	}

private:

};

#endif