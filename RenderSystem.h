#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include <vector>
#include <map>

#include "shaderClass.h"
#include "Object.h"
#include "Camera.h"
#include "tFunctions.h"

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

	Shader* MeshShader;
	Shader* ImageBoxShader;
	Shader* BoxShader;
	Shader* Text2DShader;
	Shader* ParticleShader;

	RenderSystem() {

	}
	/*
	void AddRenderable() {

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
	void Render3DScene(Camera& camera) {
		glm::mat4 proj = camera.GetProjectionMatrix(90.0f, 0.05f, 2000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//do a loop for each shader while throwing non opaques into a map and then looop through the non opaques and render + set shaders individually

		//MESH

		std::map<float,Renderable*> nonopaque;

		PrepareMeshShader(camera);

		for (Renderable* r : renderables) {

			//render opaque meshes and sort the non opaques in this loop

			if (r->opaque) {
				if (r->shadertype = Renderable::MeshShader) {
					r->Render(*MeshShader);
				}
				else {
					//nonopaque[Magnitude2()] = r;
				}
			}
			else {
				
			}
		}

		//
		PrepareParticleShader(camera);
		for (Renderable* r : renderables) {
			if (r->opaque) {
				if (r->shadertype = Renderable::MeshShader) {
					r->Render(*ParticleShader);
				}
			}
			else {

			}
		}

		for (std::map<float, Renderable*>::reverse_iterator it = nonopaque.rbegin(); it != nonopaque.rend(); ++it)
		{
			RenderRenderable((*it),camera);
		}
	}

	void Render(Camera& camera) {
		Render3DScene(camera);
		//GUI
		Render2DScene(camera);

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

	std::vector<Renderable*> renderables;
	*/
};

#endif