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
#include "common.h"
#include "Engine.h"
#include "Camera.h"
#include "t.h"
#include "tFunctions.h"

enum ColliderType {
	SPHERE,
	PLANE
};


class p {
friend class Physics;

public:
	t_package* pointer = nullptr;

	p() {
		pointer = nullptr;
		std::cout << "PHYSICS OBJECT INITIALIZED WITHOUT POINTER IN CONSTRUCTOR!!!\n";
		//thisisprettybad
		//dont use it if you dont gotta
	};

	p(t_package* t) {
		pointer = t;
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
	bool collision = true;
	bool velocity = true;

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
	p* ObjA;
	p* ObjB;

	Collision(p* a, p* b) : ObjA(a), ObjB(b) {

	};
};

class Physics  {
private:
	std::vector<p*> objects;
	glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
public:

	Physics() {

	}

	int GetObjectAmount() const noexcept {
		return objects.size();
	}

	void AddObject(p* p) {
		objects.push_back(p);
	}

	p* PPointerOfObject(t_package* pointer) const {
		for (p* p : objects) {
			if (p->pointer == pointer) return p;
		}
		return nullptr;
	}

	void RemoveP(p* p) {
		//DEPRECATED
		objects.erase(std::find(objects.begin(), objects.end(), p));
	}

	void RemoveObject(t_package* pointer) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->pointer == pointer) {
				delete objects[i];
				objects.erase(objects.begin() + i);
				return;
			}
		}
	}

	void Step(float dt) {
		for (p* p : objects) {
			if (p->velocity == true) {
				p->force += p->mass * gravity
					//+ -p->linearvelocity/2.0f
					;
				//p->angularvelocity += -(p->angularvelocity / 2.0f)/p->mass*dt;
				p->linearvelocity += p->force / p->mass * dt;
			}
		}
		ResolveCollisions(dt);

		for (p* p : objects) {
			if (p->velocity == true) {
				p->pointer->t.TranslateBy(p->linearvelocity * dt);
				p->pointer->t.RotateByQuaternion(glm::quat(p->angularvelocity * dt));

				p->force = glm::vec3(0.0f);
			}
		}
	}
	void SolveCollisions(std::vector<Collision> collisions,float dt) {
		for (Collision collision : collisions) {
			Resolve(&collision);
		}
	};
	void ResolveCollisions(float dt) {

		std::vector<Collision> collisions;
		
		for (p* a : objects) {
			if (a->collision == false or a->velocity == false) continue;
			for (p* b : objects) {
				if ((a == b) or (b->collision == false) or (not a->velocity and not b->velocity)) continue;
				if (TNearT(a->pointer->t, b->pointer->t) and BoundingBoxInBoundingBox(a->pointer->t.GetAABB(), b->pointer->t.GetAABB())) {
					std::optional<TInTInfo> e = TInT(a->pointer->t, b->pointer->t);
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

	void Resolve(Collision* c);

};

#endif
