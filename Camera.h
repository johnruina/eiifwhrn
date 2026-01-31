#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <algorithm>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "t.h"
#include "tFunctions.h"
#include "shaderClass.h"
const glm::vec3 worldUp = { 0.0f,1.0f,0.0f };

class Camera {
public:
	bool lockedcursor = true;
	int width;
	int height;
	t_package t;

	const float originalspeed = 0.08f;
	float speed = 0.08f;
	float sensitivity = 0.08f;

	Camera(int width, int height, glm::vec3 position);

	void UpdateWidthHeight(int nwidth, int nheight){
		width = nwidth;
		height = nheight;
	}

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader) {
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::lookAt(t.GetTranslation(), t.GetTranslation() + t.GetFrontVector(),worldUp);
		proj = glm::perspective(glm::radians(FOVdeg), ((float)width / (float)height), nearPlane, farPlane);
		shader.SetMat4("proj", proj);
		shader.SetMat4("view", view);
		shader.Set3F("viewPos", t.GetTranslation());
	};

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(t.GetTranslation(), t.GetTranslation() + t.GetFrontVector(), worldUp);
	}

	glm::mat4 GetProjectionMatrix(float FOVdeg, float nearPlane, float farPlane) {
		return glm::perspective(glm::radians(FOVdeg), ((float)width / (float)height), nearPlane, farPlane);
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		t.RotateByQuaternion(glm::angleAxis(glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f)));
		t.RotateByQuaternion(glm::angleAxis(glm::radians(yoffset),t.GetRightVector()));
		t.RotateToQuaternion(LookAt(t.GetFrontVector()));
		t.NormalizeRotation();
		//OPTIMIZATIONS ARE TO BE HAD
		
	}

private:

};

#endif // !CAMERA_CLASS_H
