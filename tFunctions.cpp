#include "tFunctions.h"

glm::vec3 CalculateTriangleNormal(Triangle t) {
	return glm::normalize(glm::cross(t.b - t.a, t.c - t.a));
}

float VolumeOfTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	return glm::dot(p1, glm::cross(p2, p3)) / 6.0f;
}

std::optional<glm::vec3> RayIntersectsTriangle(const Ray& ray,
	const Triangle triangle)
{

	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	const glm::vec3 ray_origin = ray.origin;
	const glm::vec3 ray_direction = ray.direction;

	glm::vec3 edge1 = triangle.b - triangle.a;
	glm::vec3 edge2 = triangle.c - triangle.a;
	glm::vec3 ray_cross_e2 = cross(ray_direction, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return {};    // This ray is parallel to this triangle.

	float inv_det = 1.0 / det;
	glm::vec3 s = ray_origin - triangle.a;
	float u = inv_det * dot(s, ray_cross_e2);

	if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon))
		return {};

	glm::vec3 s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(ray_direction, s_cross_e1);

	if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
		return {};

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = inv_det * dot(edge2, s_cross_e1);

	if (t > epsilon) // ray intersection
	{
		return  glm::vec3(ray_origin + ray_direction * t);
	}
	else // This means that there is a line intersection but not a ray intersection.
		return {};
}

std::optional<glm::vec3> IsRayInT(const Ray& ray, t_package t) {
	const BoundingBox aabb = t.GetRotationlessAABB();
	const glm::vec3 ray_origin = glm::conjugate(t.GetRotationQuaternion()) * ray.origin;
	const glm::vec3 ray_direction = glm::conjugate(t.GetRotationQuaternion()) * ray.direction;



	return {};
}

bool IsBoundingBoxInBoundingBox(const BoundingBox bb1, const BoundingBox bb2) {
	glm::vec3 bb1min = bb1.min;
	glm::vec3 bb1max = bb1.max;
	glm::vec3 bb2min = bb2.min;
	glm::vec3 bb2max = bb2.max;
	return (bb1min[0] < bb2max[0] and bb2min[0] < bb1max[0] and bb1min[1] < bb2max[1] and bb2min[1] < bb1max[1] and bb1min[2] < bb2max[2] and bb2min[2] < bb1max[2]);
}

bool IsBoundingAxisInBoundingAxis(BoundingAxis ba1, BoundingAxis ba2) {
	return ba1.min < ba2.max and ba2.min < ba1.max;
}

bool TInT(t_package& t1, t_package& t2) {
	glm::vec3 t1size = t1.GetScale();
	glm::vec3 t2size = t2.GetScale();
	glm::vec3 t1pos = t1.GetTranslation();
	glm::vec3 t2pos = t2.GetTranslation();
	glm::quat t1conquat = glm::conjugate(t1.GetRotationQuaternion());
	glm::quat t2conquat = glm::conjugate(t2.GetRotationQuaternion());


	glm::vec3 t1posrelative = t1pos - t2pos;
	glm::vec3 t2posrelative = t2pos - t1pos;

	t_package t1c = t1;
	t_package t2c = t2;

	t1c.RotateByQuaternion(t2conquat);
	t1c.TranslateTo(t2conquat * t1posrelative);
	t2c.RotateByQuaternion(t1conquat);
	t2c.TranslateTo(t1conquat * t2posrelative);

	if (not IsBoundingBoxInBoundingBox({ {-t1size[0] / 2.0f,-t1size[1] / 2.0f,-t1size[2] / 2.0f} ,{t1size[0] / 2.0f + t1pos[0],t1size[1] / 2.0f + t1pos[1],t1size[2] / 2.0f + t1pos[2]} }, t2c.GetAABB())) return false;
	if (not IsBoundingBoxInBoundingBox({ {-t2size[0] / 2.0f,-t2size[1] / 2.0f,-t2size[2] / 2.0f} ,{t2size[0] / 2.0f + t2pos[0],t2size[1] / 2.0f + t2pos[1],t2size[2] / 2.0f + t2pos[2]} }, t1c.GetAABB())) return false;

	return true;
}

glm::quat AngleAxis(glm::vec3 axis, float angle) {
	float halfangle = angle * 0.5f;

	glm::quat q;

	float s = sin(halfangle);

	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.z = 0.0f;
	q.w = cos(halfangle);
	return q;
}

glm::vec3 LookAtVector(glm::vec3 start, glm::vec3 end) {

	return glm::normalize(end - start);

}

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

glm::quat LookAt(glm::vec3 direction) {
	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);

	glm::vec3 desiredUp = { 0.0f,1.0f,0.0f };
	glm::vec3 right = cross(direction, desiredUp);
	desiredUp = cross(right, direction);

	// Because of the 1rst rotation, the up is probably completely screwed up.
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	return rot2 * rot1;
}

glm::quat LookAt(glm::vec3 start, glm::vec3 end) {

	return LookAt(end - start);
}
