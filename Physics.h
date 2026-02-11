#ifndef PHYSICS_CLASS
#define PHYSICS_CLASS

#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <cmath>
#include <optional>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//#include "Debug.h"



#include "t.h"
#include "tFunctions.h"

enum ColliderType {
	SPHERE,
	PLANE
};


class p_package {
friend class Physics;

public:
	t_package* t;

	p_package() {
		t = nullptr;
		linearvelocity = glm::vec3(0.0f);
		force = glm::vec3(0.0f);

		velocity = true;
		collision = true;
	};
	
	p_package(t_package* nt) {
		t = nullptr;
		if (nt == nullptr) {
			std::cout << "nullptr recieved in p package initalizer\n";
		}
		else {
			t = nt;
		}
		mass = 1.0f;
		linearvelocity = glm::vec3(0.0f);
		force = glm::vec3(0.0f);

		velocity = true;
		collision = true;
	};

	void SetLinearVelocity(glm::vec3 nf) {
		linearvelocity = nf;
	}

	void EnableCollision() {
		collision = true;
	}
	
	void DisableCollision() {
		collision = false;
	}

	void EnableVelocity() {
		velocity = true;
	}

	void DisableVelocity () {
		velocity = false;
	}

	float GetMass() const noexcept {
		return mass;
	}

	void SetMass(float newmass) {
		mass = newmass;
	}


public:
	bool collision;
	bool velocity;

	float restitution = 1.0f;

	glm::vec3 linearvelocity;
	float mass = 1.0f;
	glm::vec3 force = {0.0f,0.0f,0.0f};
	//it's time for me to lose my head
	glm::vec3 angularvelocity = { 0.0f,0.0f,0.0f }; //guhhhh
};

struct Collision {
	glm::vec3 POI; // point of intersection
	glm::vec3 CN; // COLLISION NORMAL
	p_package* ObjA;
	p_package* ObjB;

	Collision(p_package* a, p_package* b) : ObjA(a), ObjB(b) {

	};
};

class Physics  {
private:
	std::vector<p_package*> objects;
	glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
public:
	int GetObjectAmount() const noexcept {
		return objects.size();
	}

	void AddObject(p_package* p) {
		objects.push_back(p);
	}

	void RemoveObject(p_package* p) {
		objects.erase(std::find(objects.begin(), objects.end(), p));
	}

	void Step(float dt) {
		for (p_package* p : objects) {
			if (p->velocity == true) {
				p->force += p->mass * gravity;
				p->linearvelocity += p->force / p->mass * dt;

				p->t->TranslateBy(p->linearvelocity * dt);
				p->t->RotateByQuaternion(glm::quat( p->angularvelocity * dt));

				p->force = glm::vec3(0.0f);
			}

		}
		ResolveCollisions(dt);
	}
	void SolveCollisions(std::vector<Collision> collisions,float dt) {
		for (Collision collision : collisions) {
			Resolve(&collision);
		}
	};
	void ResolveCollisions(float dt) {

		std::vector<Collision> collisions;

		for (p_package* a : objects) {
			if (a->collision == false or a->velocity == false) continue;
			for (p_package* b : objects) {

				if ((a == b) or (b->collision == false) or (not a->velocity and not b->velocity)) continue;

				if (TNearT(*a->t, *b->t) and BoundingBoxInBoundingBox(a->t->GetAABB(), b->t->GetAABB())) {
					std::optional<TInTInfo> e = TInT(*a->t, *b->t);
					if (e.has_value()) {
						Collision tp(a,b);
						tp.POI = e.value().POI;
						tp.CN = e.value().CN;
						collisions.push_back(tp);
					}
				}
			}
		}

		SolveCollisions(collisions,dt);

	}

	void Resolve(Collision* c) {

		p_package* a = c->ObjA;
		p_package* b = c->ObjB;

		glm::vec3 ascale = a->t->GetScale();
		glm::vec3 bscale = b->t->GetScale();

		glm::vec3 relativevelocity = a->linearvelocity - b->linearvelocity;
		glm::vec3 collisionnormal = c->CN;

		glm::vec3 relativecola = c->POI - a->t->GetTranslation();
		glm::vec3 relativecolb = c->POI - b->t->GetTranslation();

		constexpr float onedividedbytwelve = 0.08333333333f;

		glm::mat3 inverseinertiatensora(
			onedividedbytwelve * a->mass * (ascale.y * ascale.y + ascale.z * ascale.z),0.0f,0.0f,
			0.0f, onedividedbytwelve * a->mass * (ascale.x * ascale.x + ascale.z * ascale.z),0.0f,
			0.0f,0.0f, onedividedbytwelve * a->mass * (ascale.x * ascale.x + ascale.y * ascale.y)
		);


		glm::mat3 inverseinertiatensorb(
			onedividedbytwelve * b->mass * (bscale.y * bscale.y + bscale.z * bscale.z), 0.0f, 0.0f,
			0.0f, onedividedbytwelve * b->mass * (bscale.x * bscale.x + bscale.z * bscale.z), 0.0f,
			0.0f, 0.0f, onedividedbytwelve * b->mass * (bscale.x * bscale.x + bscale.y * bscale.y)
		);

		inverseinertiatensora = glm::inverse(inverseinertiatensora);
		inverseinertiatensorb = glm::inverse(inverseinertiatensorb);

		float inversemass1 = 1.0f / a->GetMass();
		float inversemass2 = 1.0f / b->GetMass();

		float totalvelocity = glm::dot( - 1 * (1 + a->restitution * b->restitution) * relativevelocity,collisionnormal);
		//foolish human, i laced yo shit
		float impulse = totalvelocity /( glm::dot(collisionnormal,collisionnormal*(inversemass1 + inversemass2)) + glm::dot( glm::cross(inverseinertiatensora * glm::cross(relativecola,collisionnormal),relativecola) + glm::cross(inverseinertiatensorb * glm::cross(relativecolb, collisionnormal), relativecolb), collisionnormal));

		float frictionfactor = 0.5f;
		glm::vec3 frictionvector = glm::cross(glm::cross(collisionnormal,relativevelocity),collisionnormal);

		std::cout << impulse << '\n';

		if (a->velocity) {
			a->linearvelocity +=inversemass1 * impulse * (collisionnormal+frictionfactor*frictionvector);
			a->angularvelocity +=inverseinertiatensora * impulse * (glm::cross(relativecola, (collisionnormal + frictionfactor * frictionvector)));
		}

		if (b->velocity) {
			b->linearvelocity -= inversemass2 * impulse * (collisionnormal + frictionfactor * frictionvector);
			b->angularvelocity -= inverseinertiatensorb * (glm::cross(relativecolb, impulse * (collisionnormal + frictionfactor * frictionvector)));
		}

	}

};

#endif
