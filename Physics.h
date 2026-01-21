#ifndef T_CLASS
#define T_CLASS

#include <iostream>
#include <random>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "t.h"

const glm::vec3 gravity = glm::vec3(0, -9.81f, 0);

class p_package {
friend class Physics;

public:


	
private:
	glm::vec3 linearvelocity;
	float mass;
	glm::vec3 force;
	t_package* t;
	//angular velocity to come at a much later date
};

class Physics  {
private:
	std::vector<p_package*> objects;
public:
	void AddObject(p_package* p) {
		objects.push_back(p);
	}

	void RemoveObject(p_package* p) {
		
	}

	void Step() {
		for (p_package* p : objects) {
			p->force += p->mass * gravity; // apply a force

			p->linearvelocity += p->force / p->mass * 1.0f/60.0f;
			p->t->TranslateBy(p->linearvelocity * 1.0f / 60.0f);

			p->force = glm::vec3(0.0f); // reset net force at the end
		}
	}
};

#endif
