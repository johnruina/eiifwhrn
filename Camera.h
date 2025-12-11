#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <algorithm>

#include "shaderClass.h"

class Camera {

public:
	glm::vec3 worldUp = { 0.0f,1.0f,0.0f };
	bool lockedcursor = false;
	int width;
	int height;
	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	glm::vec3 position;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;

	//PLACEHOLDER TILL I MAKE A KEYBOARD SYSTEM OR SMTH
	bool FirstTab = true;

	const float originalspeed = 0.01f;
	float speed = 0.1f;
	float sensitivity = 0.1f;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader) {
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::lookAt(position, position + Front, glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
		shader.SetMat4("proj", proj);
		shader.SetMat4("view", view);
	};

	void SetPosition(glm::vec3 nposition) {
		position = nposition;
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

private:
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(front, worldUp));
		Up = glm::normalize(glm::cross(front, Right));
		//std::cout << std::to_string(Front[0]) << " " << std::to_string(Front[1]) << " " << std::to_string(Front[2]) << '\n';
	}
};

#endif // !CAMERA_CLASS_H
