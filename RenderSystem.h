#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include <vector>
#include <map>

#include "common.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "FBO.h"
#include "shaderClass.h"
#include "Object.h"
#include "Camera.h"
#include "Cubemap.h"
#include "QuadVertices.h"

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

class Renderable {
public:

	enum ShaderType {
		MeshShader,
		ImageBoxShader,
		BoxShader,
		TextShader,
		ParticleShader
	};

	ShaderType shadertype = ShaderType::MeshShader;
	bool opaque = true;
	bool gui = false;
	Renderable() {

	}

	virtual void Render(Shader& ShaderProgram) {

	}
};

class RenderSystem {
public:	

	//FRAMEBUFFER STUFF
	//FRAMEBUFFER
	FBO framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	VAO quadVAO;
	VBO quadVBO;

	VAO skyboxVAO;
	VBO skyboxVBO;

	Shader* SkyboxShader;
	Shader* ScreenShader;

	Shader* MeshShader;
	Shader* ImageBoxShader;
	Shader* BoxShader;
	Shader* Text2DShader;
	Shader* ParticleShader;

	Cubemap* skybox = nullptr;



	RenderSystem() {
		
	}

	void Initialize() {
		skyboxVAO.GenerateID();
		skyboxVBO.GenerateID();
		skyboxVAO.Bind();
		skyboxVBO.Bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		quadVAO.GenerateID();
		quadVBO.GenerateID();
		quadVAO.Bind();
		quadVBO.Bind();
		quadVBO.BufferData(&quadVertices, sizeof(quadVertices));

		quadVAO.LinkVBO(quadVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
		quadVAO.LinkVBO(quadVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		std::vector<std::string> faces = {
		"assets/miramar_ft.tga",
		"assets/miramar_bk.tga",
		"assets/miramar_up.tga",
		"assets/miramar_dn.tga",
		"assets/miramar_rt.tga",
		"assets/miramar_lf.tga"
		};

		Cubemap* skyboxcm = new Cubemap();
		skyboxcm->FillCubemap(faces);
		skybox = skyboxcm;
		framebuffer.Bind();
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->width, window->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->width, window->height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		framebuffer.Unbind();
	}
	
	void AddRenderable(Renderable* r) {
		renderables.emplace_back(r);
	}

	void BindCorrectShader(Renderable* renderable, Camera& camera) {
		if (renderable->shadertype == Renderable::MeshShader) PrepareMeshShader(camera);
		else if (renderable->shadertype == Renderable::MeshShader) PrepareParticleShader(camera);
	}

	Shader* ReturnCorrectShader(Renderable* renderable) {
		if (renderable->shadertype == Renderable::MeshShader) return MeshShader;
		else if (renderable->shadertype == Renderable::MeshShader) return ParticleShader;
	}

	void RenderRenderable(Renderable* renderable, Camera& camera) {
		Shader* shader = ReturnCorrectShader(renderable);
		shader->Activate();
		renderable->Render(*shader);
	}
	void Render(Camera& camera) {

		glm::mat4 proj = camera.GetProjectionMatrix(90.0f, 0.05f, 2000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		framebuffer.Bind();

		glEnable(GL_DEPTH_TEST);

		//IMPORTANT
		glClearColor(212. / 255., 223. / 255., 232. / 255., 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////////////////////////////////////SKYBOX////////////////////////////////////

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_DEPTH_TEST);  // change depth function so depth test passes when values are equal to depth buffer's content
		SkyboxShader->Activate();
		SkyboxShader->SetMat4("proj", proj);
		SkyboxShader->SetMat4("view", glm::mat4(glm::mat3(view)));
		// skybox cube
		skyboxVAO.Bind();
		glActiveTexture(GL_TEXTURE0);
		skybox->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);

		//do a loop for each shader while throwing non opaques into a map and then looop through the non opaques and render + set shaders individually

		//MESH

		//std::map<float,Renderable*> nonopaque;

		PrepareMeshShader(camera);

		MeshShader->Set3F("dirLight.ambient", glm::vec3(0.5f));
		MeshShader->Set3F("dirLight.diffuse", glm::vec3(0.5f));
		MeshShader->Set3F("dirLight.specular", glm::vec3(0.5f));
		MeshShader->Set3F("dirLight.direction", glm::vec3(0.0f, -1.0f, 1.0f));

		for (Renderable* r : renderables) {
			//render opaque meshes and sort the non opaques in this loop
			if (r->shadertype == Renderable::ShaderType::MeshShader) {
				r->Render(*MeshShader);
			}

		}

		//
		PrepareParticleShader(camera);
		for (Renderable* r : renderables) {
			if (r->shadertype == Renderable::ShaderType::ParticleShader) {
				r->Render(*ParticleShader);
			}
		}
		/*
		for (std::map<float, Renderable*>::reverse_iterator it = nonopaque.rbegin(); it != nonopaque.rend(); ++it)
		{
			RenderRenderable((*it).second,camera);
		}*/
		
		//FRAMEBUFFER
		framebuffer.Unbind();
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ScreenShader->Activate();

		//ScreenShader.Set1F("saturation", sin((float)frame/15) * 1.0f + 1.0f);
		quadVAO.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//GUI


		PrepareBoxShader(camera);
		for (Renderable* r : renderables) {
			if (r->shadertype == Renderable::BoxShader) {
				r->Render(*BoxShader);
			}
		}

		

		glfwSwapBuffers(window->handle);
	}

private:

	void PrepareMeshShader(Camera& camera) {
		MeshShader->Activate();

		glm::mat4 proj = camera.GetProjectionMatrix(90.0f, 0.05f, 2000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		MeshShader->SetMat4("proj", proj);
		MeshShader->SetMat4("view", view);
		MeshShader->Set3F("viewPos", camera.t.GetTranslation());
	}

	void PrepareParticleShader(Camera& camera) {
		ParticleShader->Activate();

		glm::mat4 proj = camera.GetProjectionMatrix(90.0f, 0.05f, 2000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		ParticleShader->SetMat4("proj", proj);
		ParticleShader->SetMat4("view", view);
	}

	void PrepareBoxShader(Camera& camera) {
		BoxShader->Activate();
		BoxShader->Set2F("screenSize", { window->width ,window->height });
	}

	void PrepareImageBoxShader(Camera& camera) {
		ImageBoxShader->Activate();
		ImageBoxShader->Set2F("screenSize", { window->width ,window->height });
	}

	std::vector<Renderable*> renderables;
	
};

#endif