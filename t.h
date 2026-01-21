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
#include "Structures.h"

class t_package {

public:

	t_package() {
		pos = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		orient = glm::quat();
	}

	BoundingBox GetRotationlessAABB() {
		BoundingBox tr;
		tr.min = pos - scale/2.0f;
		tr.max = pos + scale/2.0f;
		return tr;
	}

	BoundingBox GetAABB() {
		std::vector<glm::vec3> corners = {
			{0.5f,0.5f,0.5f},
			{0.5f,-0.5f,0.5f},
			{0.5f,-0.5f,-0.5f},
			{0.5f,0.5f,-0.5f},
			{-0.5f,0.5f,0.5f},
			{-0.5f,-0.5f,0.5f},
			{-0.5f,0.5f,-0.5f},
			{-0.5f,-0.5f,-0.5f},
		};

		glm::vec3 min;
		glm::vec3 max;
		bool init = false;
		for (glm::vec3& corner : corners) {
			corner *= scale;
			corner = orient * corner;
			corner += pos;
			if (not init) {
				min = corner;
				max = corner;
				init = true;
			}
			else {
				min = { std::min(min.x, corner.x) ,std::min(min.y, corner.y), std::min(min.z, corner.z) };
				max = { std::max(max.x, corner.x) ,std::max(max.y, corner.y), std::max(max.z, corner.z) };
			}
			

		}
		BoundingBox tr;
		tr.min = min;
		tr.max = max;
		return tr;
	}

	BoundingAxis GetBoundingAxis(int axis) {
		std::vector<glm::vec3> corners = {
		{0.5f,0.5f,0.5f},
		{0.5f,-0.5f,0.5f},
		{0.5f,-0.5f,-0.5f},
		{0.5f,0.5f,-0.5f},
		{-0.5f,0.5f,0.5f},
		{-0.5f,-0.5f,0.5f},
		{-0.5f,0.5f,-0.5f},
		{-0.5f,-0.5f,-0.5f},
		};

		float min;
		float max;
		bool init = false;
		for (glm::vec3& corner : corners) {
			corner *= scale;
			corner = orient * corner;
			corner += pos;
			if (not init) {
				min = corner[axis];
				max = corner[axis];
				init = true;
			}
			else {
				min =std::min(min, corner[axis]);
				max =std::max(max, corner[axis]);
			}


		}
		BoundingAxis tr;
		tr.min = min;
		tr.max = max;
		return tr;
	}

	void LookAt(glm::vec3 eye, glm::vec3 target) noexcept {
		TranslateTo(eye);
		orient = -orient * target;
	}

	void TranslateBy(glm::vec3 e) noexcept {
		pos += e;
	}

	void TranslateTo(glm::vec3 e) noexcept {
		pos = e;
	}

	void ScaleBy(glm::vec3 e) noexcept {
		scale *= e;
	}

	void ScaleBy(GLfloat e) noexcept {
		scale *= e;
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
		orient = q * orient;
	}

	void NormalizeRotation() noexcept {
		orient = glm::normalize(orient);
	}

	void RotateByQuaternionCumulate(glm::quat q) noexcept {
		orient = orient * q;
	}

	void RotateByEulerAngles(glm::vec3 e) noexcept {
		RotateByQuaternion(glm::quat(e));
	}

	glm::vec3 GetUpVector() const noexcept {
		glm::vec3 uppoint(0.0f, 1.0f,0.0f);
		return orient * uppoint;
	}

	glm::vec3 GetRightVector() const noexcept {
		glm::vec3 rightpoint(1.0f,0.0f, 0.0f);
		return orient * rightpoint;
	}

	glm::vec3 GetFrontVector() const noexcept {
		
		glm::vec3 frontpoint(0.0f,0.0f,1.0f);
		return orient * frontpoint;
	}

	glm::vec3 GetScale() const noexcept {
		return scale;
	}

	glm::vec3 GetTranslation() const noexcept {
		return pos;
	};

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

	glm::quat GetRotationQuaternion() const noexcept {
		return orient;
	}

	glm::vec3 GetRotationEulerAngles() const noexcept {
		return glm::eulerAngles(orient);
	}

private:
	glm::vec3 pos;
	glm::vec3 scale;
	glm::quat orient;
};

#endif
