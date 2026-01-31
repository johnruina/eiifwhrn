#ifndef PHYSICS_CLASS
#define PHYSICS_CLASS

#include <iostream>
#include <random>
#include <vector>
#include <memory>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

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
		mass = 1.0f;
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

private:
	bool collision;
	bool velocity;

	glm::vec3 linearvelocity;
	float mass;
	glm::vec3 force;
	//angular velocity to come at a much later date
};

struct CollisionPoints {
	glm::vec3 A; // Furthest point of A into B
	glm::vec3 B; // Furthest point of B into A
	glm::vec3 Normal; // B – A normalized
	float Depth;    // Length of B – A
	bool HasCollision;
};

struct Collision {
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

				p->force = glm::vec3(0.0f);
			}

		}
		ResolveCollisions(dt);
	}
	void SolveCollisions(std::vector<Collision> collisions,float dt) {
		for (Collision collision : collisions) {
			if (not collision.ObjB->velocity) {
				OneSidedResolve(collision.ObjA, collision.ObjB);
			}
			else {
				MutualResolve(collision.ObjA, collision.ObjB);
			}
		}
	};
	void ResolveCollisions(float dt) {
		std::vector<Collision> collisions;

		for (p_package* a : objects) {
			if (a->collision == false or a->velocity == false) continue;
			for (p_package* b : objects) {

				if ((a == b) or (b->collision == false) or (not a->velocity and not b->velocity)) continue;

				if (IsBoundingBoxInBoundingBox(a->t->GetAABB(), b->t->GetAABB()) and TInT(*a->t, *b->t)) {
					collisions.emplace_back(a,b);
				}
			}
		}

		SolveCollisions(collisions,dt);
	}

	void OneSidedResolve(p_package* a, p_package* b) {
		t_package* at = a->t;
		t_package* bt = b->t;
		glm::vec3 center = at->GetTranslation();
		glm::vec3 directionvec = LookAtVector(at->GetTranslation(), bt->GetTranslation());

		a->linearvelocity *= -1;
		
	}

	void MutualResolve(p_package* a, p_package* b) {
		//MOVE FROM CENTER, DONT ACCOUNT FOR MASS
		t_package* at = a->t;
		t_package* bt = b->t;
		glm::vec3 center = (at->GetTranslation() + bt->GetTranslation())/2.0f;
		glm::vec3 directionvec = LookAtVector(at->GetTranslation(), bt->GetTranslation());
		at->TranslateTo(center - directionvec * at->GetScale()/2.0f);
		bt->TranslateTo(center + directionvec * bt->GetScale() / 2.0f);
	}

};

#endif
