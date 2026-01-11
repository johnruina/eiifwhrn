
#ifndef FBO_CLASS_H
#define FBO_CLASS_H

#include <glad/glad.h>

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};
// cube

class FBO {
public:
	GLuint ID;
	FBO() {
		glGenFramebuffers(1, &ID);
	};
	void Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	};
	void Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};
	void Delete() {
		glDeleteFramebuffers(1, &ID);
	};
};

#endif