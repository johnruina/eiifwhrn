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

class t_package {

public:



	void TranslateBy(glm::vec3 e) noexcept {
		t = glm::translate(t, e);
	}

	void TranslateTo(glm::vec3 e) noexcept {
		t[3][0] = e.x;
		t[3][1] = e.y;
		t[3][2] = e.z;
	}

	void ScaleBy(glm::vec3 e) noexcept {
		t = glm::scale(t, e);
	}

	void ScaleBy(GLfloat e) noexcept {
		t = glm::scale(t, glm::vec3(e));
	}

	void ScaleTo(glm::vec3 e) noexcept {
		ScaleBy(glm::vec3(1.0f) / GetScale());
		ScaleBy(e);
	}

	void ScaleTo(GLfloat e) noexcept {
		ScaleBy(glm::vec3(1.0f) / GetScale());
		ScaleBy(e);
	}

	void SetMatrix(glm::mat4 newt) noexcept {
		t = newt;
	}

	glm::vec3 GetScale() noexcept {
		glm::vec3 size;
		size.x = glm::length(glm::vec3(t[0])); // Basis vector X
		size.y = glm::length(glm::vec3(t[1])); // Basis vector Y
		size.z = glm::length(glm::vec3(t[2])); // Basis vector Z
		return size;
	}

	glm::vec3 GetPosition() const noexcept {
		return { t[3][0],t[3][1] ,t[3][2] };
	};

	glm::mat4 GetMatrix() const noexcept {
		return t;
	}

private:
	glm::mat4 t;
};

#endif
