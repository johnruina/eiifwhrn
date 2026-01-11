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

	t_package() {
		pos = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		orient = glm::quat();
	}

	void TranslateBy(glm::vec3 e) noexcept {
		pos += e;
	}

	void TranslateTo(glm::vec3 e) noexcept {
		pos = e;
	}

	void ScaleBy(glm::vec3 e) noexcept {
		scale += e;
	}

	void ScaleBy(GLfloat e) noexcept {
		scale += glm::vec3(e);
	}

	void ScaleTo(glm::vec3 e) noexcept {
		scale = e;
	}

	void ScaleTo(GLfloat e) noexcept {
		scale = glm::vec3(e);
	}

	void RotateToQuaternion(glm::quat q) noexcept {
		orient = q;
	}

	void RotateByQuaternion(glm::quat q) noexcept {
		orient = orient* q;
	}

	void RotateByEulerAngles(glm::vec3 e) noexcept {
		RotateByQuaternion(glm::quat(e));
	}

	void LookAt(glm::vec3 pos) noexcept {

	}

	glm::vec3 GetUpVector() const noexcept {

	}

	glm::vec3 GetRightVector() const noexcept {

	}

	glm::vec3 GetFrontVector() const noexcept {


	}

	glm::vec3 GetScale() const noexcept {
		return scale;
	}

	glm::vec3 GetTranslation() const noexcept {
		return pos;
	};

	glm::quat GetRotation() const noexcept {
		return orient;
	}

	void CopyMatrixTo(t_package& target) const noexcept {

		target.scale = scale;
		target.orient = orient;
		target.pos = pos;
	}

	glm::mat4 GetMatrix() const noexcept {
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotMat = glm::mat4_cast(orient);
		glm::mat4 translMat = glm::translate(glm::mat4(1.0f), pos);
		return translMat * rotMat * scaleMat;
	}

private:
	glm::vec3 pos;
	glm::vec3 scale;
	glm::quat orient;
};

#endif
