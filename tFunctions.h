#ifndef T_FUNCTIONS
#define T_FUNCTIONS

#include <iostream>
#include <random>
#include <vector>
#include <optional>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "t.h"
#include "Structures.h"

glm::vec3 CalculateTriangleNormal(Triangle t);

float SquaredMagnitude(const glm::vec3& v);

float Magnitude(const glm::vec3& v);

float SquaredPerpendicularMagnitude(glm::vec3 off, glm::vec3 line);

float VolumeOfTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
std::optional<glm::vec3> RayIntersectsTriangle(const Ray& ray,
	const Triangle triangle);

std::optional<glm::vec3> IsRayInT(const Ray& ray, t_package t);

bool BoundingBoxInBoundingBox(const BoundingBox bb1, const BoundingBox bb2);
bool BoundingAxisInBoundingAxis(BoundingAxis ba1, BoundingAxis ba2);

std::optional<std::pair<float, glm::vec3>>  TAxisCollidesT(std::vector < glm::vec3 >& t1, std::vector < glm::vec3 >& t2, glm::vec3 axis);

bool TNearT(t_package& t1, t_package& t22);
std::optional<glm::vec3> TInT(t_package& t1, t_package& t2);

glm::quat AngleAxis(glm::vec3 axis, float angle);
glm::vec3 LookAtVector(glm::vec3 start, glm::vec3 end);

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
glm::quat LookAt(glm::vec3 direction);

glm::quat LookAt(glm::vec3 start, glm::vec3 end);

#endif