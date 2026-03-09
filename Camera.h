#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <algorithm>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "common.h"
#include "Window.h"
#include "t.h"
#include "tFunctions.h"
#include "shaderClass.h"
const glm::vec3 worldUp = { 0.0f,1.0f,0.0f };

class Camera {
public:
	bool lockedcursor = true;
	t t;

	float yaw = 0.0f;
	float pitch = 0.0f;

	const float originalspeed = 0.08f;
	float speed = 0.08f;
	float sensitivity = 0.001f;

	Camera() {
	
	};

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader) {
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::lookAt(t.GetTranslation(), t.GetTranslation() + t.GetFrontVector(),worldUp);
		proj = glm::perspective(glm::radians(FOVdeg), ((float)window->width / (float)window->height), nearPlane, farPlane);
		shader.SetMat4("proj", proj);
		shader.SetMat4("view", view);
		shader.Set3F("viewPos", t.GetTranslation());
	};

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(t.GetTranslation(), t.GetTranslation() + t.GetFrontVector(), worldUp);
	}

	glm::mat4 GetProjectionMatrix(float FOVdeg, float nearPlane, float farPlane) {
		return glm::perspective(glm::radians(FOVdeg), ((float)window->width / (float)window->height), nearPlane, farPlane);
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		pitch = glm::clamp(pitch, -glm::radians(89.0f), glm::radians(89.0f));
		t.RotateToEulerAngles({pitch,yaw,0.0f});
		t.NormalizeRotation();
		//OPTIMIZATIONS ARE TO BE HAD
		
	}

private:

};

#endif // !CAMERA_CLASS_H
