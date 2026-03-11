#ifndef RIG_CLASS
#define RIG_CLASS

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Mesh.h"

class Bone {
	glm::mat4 local_transform;
	std::string name;
	int ID;
};

class Rig : public Mesh {
public:
	std::vector<Bone> bones;
private:

};

#endif