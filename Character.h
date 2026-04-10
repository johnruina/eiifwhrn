#ifndef CHARACTER_CLASS
#define CHARACTER_CLASS

#include <vector>

#include "t.h"
#include "Rig.h"
#include "Object.h"

class Component {
	
};

class CharacterBody : public t_package {
public:
	Rig* rig;
private:

};	

class CharacterMind {

};

class Character: public Object, public Renderable {
public:
	CharacterMind* mind;
	CharacterBody* body;

	Character() {
		body = new CharacterBody();
		mind = nullptr;
		shadertype = ShaderType::RigShader;
	}

	void Render(Shader& shader) override {
		body->rig->Render(shader,body->t.GetMatrix());
	}

private:

};

#endif
