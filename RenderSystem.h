#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include <iostream>
#include <vector>
#include <map>

const float skyboxVertices[] = {
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

class Camera;
class Shader;
class FBO;
class VBO;
class VAO;
class Cubemap;

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
	Renderable();
	~Renderable();
	void AddToRenderSystem();
	virtual void Render(Shader& ShaderProgram) {

	}
};

class RenderSystem {
public:	

	//FRAMEBUFFER STUFF
	//FRAMEBUFFER
	FBO* screenfbo;
	FBO* shadowdepthmapfbo;
	unsigned int depthMaptexture;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	VAO* quadVAO;
	VBO* quadVBO;

	VAO* skyboxVAO;
	VBO* skyboxVBO;
	
	Shader* ShadowShader;
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

	void Initialize();
	
	void AddRenderable(Renderable* r) {
		renderables.emplace_back(r);
	}

	void RemoveRenderable(Renderable* r) {
		for (int i = 0; i < renderables.size(); i++) {
			if (renderables[i] == r) {
				renderables.erase(renderables.begin() + i);
				return;
			}
		}
		std::cout << "didnt find renderable to remove\n";
	}

	void BindCorrectShader(Renderable* renderable, Camera& camera) {
		if (renderable->shadertype == Renderable::MeshShader) PrepareMeshShader(camera);
		else if (renderable->shadertype == Renderable::MeshShader) PrepareParticleShader(camera);
	}

	Shader* ReturnCorrectShader(Renderable* renderable) {
		if (renderable->shadertype == Renderable::MeshShader) return MeshShader;
		else if (renderable->shadertype == Renderable::MeshShader) return ParticleShader;
	}

	void RenderRenderable(Renderable* renderable, Camera& camera);
	void Render(Camera& camera);
	
private:

	void PrepareMeshShader(Camera& camera);

	void PrepareParticleShader(Camera& camera);

	void PrepareBoxShader(Camera& camera);

	void PrepareImageBoxShader(Camera& camera);
public:
	std::vector<Renderable*> renderables;
	
};

#endif