
#ifndef OBJECTS_CLASS
#define OBJECTS_CLASS

#include <vector>
#include <string>

#include<glm/glm.hpp>

#include "t.h"
#include "Physics.h"

class Object {

public:
	std::string name;


	Object() {

	}
	
	Object(Object* parent) : parent(parent) {

	}

	std::vector<Object*> GetChildren() {
		return children;
	}

	void RemoveFromParent() {
		if (parent != nullptr) {
			parent->RemoveChild(this);
		}
	}

	void AddChild(Object* obj) {
		obj->RemoveFromParent();
		obj->parent = this;
		children.push_back(obj);
	}

	void RemoveChild(Object* obj) {

		obj->parent = nullptr;
		children.erase(GetChildIterator(obj));
	}

	std::vector<Object*>::iterator GetChildIterator(Object* obj) {
		auto it = std::find(children.begin(), children.end(), obj);
		if (it == children.end()) return children.end();
		return it;
	}

	Object* GetChild(std::string name) {

	}

	void SetParent(Object* nparent) {
		nparent->AddChild(this);
	}

	int GetChildrenAmount() {
		return children.size();
	}

	void Destroy() {
		for (auto child : children) {
			child->Destroy();
		}
		parent->RemoveChild(this);
		delete this;
	}
	
private:
	std::vector<Object*> children = {};
	Object* parent = nullptr;
};

#endif