#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <random>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"

struct Vertex {
	glm::vec3 Position;
};



class Mesh {

public:


	virtual void Render(Shader ShaderProgram) {
		
	};

	void Translate(glm::vec3 e) noexcept {
		glm::translate(t, e);
	}

	glm::vec3 GetPosition() const noexcept {
		return {t[3][0],t[3][1] ,t[3][2] };
	};
	glm::mat4 t;
private:

};