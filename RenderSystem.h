#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include <vector>

#include "shaderClass.h"
#include "Object.h"
#include "Camera.h"

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

	void AddRenderable() {

	}

	void Render(Camera& camera) {

		glm::mat4 proj = camera.GetProjectionMatrix(90.0f, 0.05f, 2000.0f);
		glm::mat4 view = camera.GetViewMatrix();



		//do a loop for each shader while throwing non opaques into a map and then looop through the non opaques and render + set shaders individually

		//MESH

		PrepareMeshShader(camera);
		
		for (Renderable* r : renderables) {

			//render opaque meshes and sort the non opaques in this loop

			if (r->opaque) {
				if (r->shadertype = Renderable::MeshShader) {
					r->Render(*MeshShader);
				}
			}
			else {

			}
		}

		//

		for (Renderable* r : renderables) {
			if (r->opaque) {
			}
			else {

			}
		}

		//GUI



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

	std::vector<Renderable*> renderables;
};

#endif