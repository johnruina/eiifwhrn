#include "tFunctions.h"

glm::vec3 CalculateTriangleNormal(Triangle t) {
	return glm::normalize(glm::cross(t.b - t.a, t.c - t.a));
}

float Magnitude(const glm::vec3& v) {
	return pow(pow(v.x,2.0f) + pow(v.y, 2.0f) + pow(v.z, 2.0f), 0.5f);
}

float SquaredMagnitude(const glm::vec3& v) {
	return pow(v.x, 2.0f) + pow(v.y, 2.0f) + pow(v.z, 2.0f);
}

//SQUARED FOR PERFORMANCE REASONS
float SquaredPerpendicularMagnitude(glm::vec3 off, glm::vec3 line)
{
	float offmag = SquaredMagnitude(off);
	return offmag * (glm::dot(off, line)/ (offmag + SquaredMagnitude(line)));
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
	glm::mat4 inverse = glm::inverse(t.GetMatrix());
	glm::vec3 ray_origin = glm::vec3(inverse * glm::vec4(ray.origin, 1.0f));
	glm::vec3 ray_direction = glm::vec3(inverse * glm::vec4(ray.direction, 0.0f));

	glm::vec3 dirfrac = glm::vec3(1.0f) / ray_direction;
		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.org is origin of ray

	glm::vec3 lb = {-0.5f,-0.5f,-0.5f};
	glm::vec3 rt = {0.5f,0.5f,0.5f };

	float t1 = (lb.x - ray_origin.x) * dirfrac.x;
	float t2 = (rt.x - ray_origin.x) * dirfrac.x;
	float t3 = (lb.y - ray_origin.y) * dirfrac.y;
	float t4 = (rt.y - ray_origin.y) * dirfrac.y;
	float t5 = (lb.z - ray_origin.z) * dirfrac.z;
	float t6 = (rt.z - ray_origin.z) * dirfrac.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us

	if (tmax < 0)
	{
		return {};
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return {};
	}

	
	return { glm::vec3(t.GetMatrix() * glm::vec4(ray_direction*tmin+ray_origin, 1.0f))};
}

bool BoundingBoxInBoundingBox(const BoundingBox bb1, const BoundingBox bb2) {
	glm::vec3 bb1min = bb1.min;
	glm::vec3 bb1max = bb1.max;
	glm::vec3 bb2min = bb2.min;
	glm::vec3 bb2max = bb2.max;
	return (bb1min[0] < bb2max[0] and bb2min[0] < bb1max[0] and bb1min[1] < bb2max[1] and bb2min[1] < bb1max[1] and bb1min[2] < bb2max[2] and bb2min[2] < bb1max[2]);
}

bool BoundingAxisInBoundingAxis(BoundingAxis ba1, BoundingAxis ba2) {
	return ba1.min < ba2.max and ba2.min < ba1.max;
}

std::optional<std::pair<float,glm::vec3>> TAxisCollidesT(std::vector < glm::vec3 >& t1, std::vector < glm::vec3 >& t2, glm::vec3 axis) {
	//FIRST RETURN IS OVERLAPPING AREA
	//SECOND RETURN IS POINT OF COLLISION
	float min1 = FLT_MAX;
	float max1 = FLT_MIN;
	float min2 = FLT_MAX;
	float max2 = FLT_MIN;

	for (glm::vec3& v : t1) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v,axis);
		min1 = glm::min(min1,perpendicularmag);
		max1 = glm::max(max1, perpendicularmag);
	}

	for (glm::vec3& v : t2) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v, axis);
		min2 = glm::min(min2, perpendicularmag);
		max2 = glm::max(max2, perpendicularmag);
	}
	if (min1 < max2 and min2 < max1) {
		float overlap = glm::min(max1, max2) - glm::max(min1, min2);
		float middle = (glm::min(max1, max2) + glm::max(min1, min2)) / 2.0f;
		return { {overlap, middle * axis} };
	}
}

bool TAxisCollidesTNoInfo(std::vector < glm::vec3 >& t1, std::vector < glm::vec3 >& t2, glm::vec3 axis) {
	//FIRST RETURN IS OVERLAPPING AREA
	//SECOND RETURN IS POINT OF COLLISION
	float min1 = FLT_MAX;
	float max1 = FLT_MIN;
	float min2 = FLT_MAX;
	float max2 = FLT_MIN;

	for (glm::vec3& v : t1) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v, axis);
		min1 = glm::min(min1, perpendicularmag);
		max1 = glm::max(max1, perpendicularmag);
	}

	for (glm::vec3& v : t2) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v, axis);
		min2 = glm::min(min2, perpendicularmag);
		max2 = glm::max(max2, perpendicularmag);
	}
	if (min1 < max2 and min2 < max1) {
		return true;
	}
	return false;
}

std::optional<AxisCollisionCN> TAxisCollidesTCN(std::vector < glm::vec3 >& t1, std::vector < glm::vec3 >& t2, glm::vec3 axis) {
	//FIRST RETURN IS OVERLAPPING AREA
	//SECOND RETURN IS POINT OF COLLISION
	float min1 = FLT_MAX;
	float max1 = FLT_MIN;
	float min2 = FLT_MAX;
	float max2 = FLT_MIN;

	for (glm::vec3& v : t1) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v, axis);
		min1 = glm::min(min1, perpendicularmag);
		max1 = glm::max(max1, perpendicularmag);
	}

	for (glm::vec3& v : t2) {
		float perpendicularmag = SquaredPerpendicularMagnitude(v, axis);
		min2 = glm::min(min2, perpendicularmag);
		max2 = glm::max(max2, perpendicularmag);
	}
	if (min1 < max2 and min2 < max1) {
		float overlap = glm::min(max1, max2) - glm::max(min1, min2);
		float middle = (glm::min(max1, max2) + glm::max(min1, min2)) / 2.0f;

		AxisCollisionCN accn;
		accn.infront = (min1+max1)/2.0f > (min2+max2)/2.0f;
		accn.POI = axis * middle;
		accn.overlap = overlap;

		return {accn};
	}
	return {};
}

bool TNearT(t_package& t1, t_package& t2)
{
	glm::vec3 posdif = t1.GetTranslation() - t2.GetTranslation();
	return t1.SquaredMagnitude() + t2.SquaredMagnitude() > (posdif.x* posdif.x+ posdif.y* posdif.y+ posdif.z* posdif.z) * 2.0f;
}

bool TInTNoInfo(t_package& t1, t_package& t2) {
	std::vector < glm::vec3 > worldvertices1 = {
	{0.5f,0.5f,0.5f},
	{0.5f,-0.5f,0.5f},
	{0.5f,0.5f,-0.5f},
	{0.5f,-0.5f,-0.5f},
	{-0.5f,0.5f,0.5f},
	{-0.5f,-0.5f,0.5f},
	{-0.5f,0.5f,-0.5f},
	{-0.5f,-0.5f,-0.5f},
	};

	std::vector < glm::vec3 > worldvertices2 = {
	{0.5f,0.5f,0.5f},
	{0.5f,-0.5f,0.5f},
	{0.5f,0.5f,-0.5f},
	{0.5f,-0.5f,-0.5f},
	{-0.5f,0.5f,0.5f},
	{-0.5f,-0.5f,0.5f},
	{-0.5f,0.5f,-0.5f},
	{-0.5f,-0.5f,-0.5f},
	};

	glm::mat4 t1mat = t1.GetMatrix();
	glm::mat4 t2mat = t2.GetMatrix();

	for (glm::vec3& v : worldvertices1) {
		v = glm::vec3(t1mat * glm::vec4(v, 1.0f));
	}
	for (glm::vec3& v : worldvertices2) {
		v = glm::vec3(t2mat * glm::vec4(v, 1.0f));
	}

	//FIRST 6 AXES

	glm::vec3 fv1 = t1.GetFrontVector();
	glm::vec3 rv1 = t1.GetRightVector();
	glm::vec3 uv1 = t1.GetUpVector();
	glm::vec3 fv2 = t2.GetFrontVector();
	glm::vec3 rv2 = t2.GetRightVector();
	glm::vec3 uv2 = t2.GetUpVector();

	std::vector<glm::vec3> axes = {
		fv1,fv2,uv1,uv2,rv1,rv2,
		glm::cross(fv1,fv2),
		glm::cross(fv1, rv2),
		glm::cross(fv1, uv2),
		glm::cross(rv1, fv2),
		glm::cross(rv1, rv2),
		glm::cross(rv1, uv2),
		glm::cross(uv1, fv2),
		glm::cross(uv1, rv2),
		glm::cross(uv1, uv2)
	};

	for (glm::vec3 axis : axes) {
		if (not TAxisCollidesTNoInfo(worldvertices1, worldvertices2, axis)) return false;
	}

	//FINAL
	return true;
}

std::optional<TInTInfo> TInT(t_package& t1, t_package& t2) {

	//FIRST IS POI, SECOND IS COLNORMAL

	std::vector < glm::vec3 > worldvertices1 = { 
		{0.5f,0.5f,0.5f},
		{0.5f,-0.5f,0.5f},
		{0.5f,0.5f,-0.5f},
		{0.5f,-0.5f,-0.5f},
		{-0.5f,0.5f,0.5f},
		{-0.5f,-0.5f,0.5f},
		{-0.5f,0.5f,-0.5f},
		{-0.5f,-0.5f,-0.5f},
	};

	std::vector < glm::vec3 > worldvertices2 = {
	{0.5f,0.5f,0.5f},
	{0.5f,-0.5f,0.5f},
	{0.5f,0.5f,-0.5f},
	{0.5f,-0.5f,-0.5f},
	{-0.5f,0.5f,0.5f},
	{-0.5f,-0.5f,0.5f},
	{-0.5f,0.5f,-0.5f},
	{-0.5f,-0.5f,-0.5f},
	};

	glm::mat4 t1mat = t1.GetMatrix();
	glm::mat4 t2mat = t2.GetMatrix();

	for (glm::vec3& v : worldvertices1) {
		v = glm::vec3(t1mat * glm::vec4(v, 1.0f));
	}
	for (glm::vec3& v : worldvertices2) {
		v = glm::vec3(t2mat * glm::vec4(v, 1.0f));
	}

	//FIRST 6 AXES

	glm::vec3 fv1 = t1.GetFrontVector();
	glm::vec3 rv1 = t1.GetRightVector();
	glm::vec3 uv1 = t1.GetUpVector();
	glm::vec3 fv2 = t2.GetFrontVector();
	glm::vec3 rv2 = t2.GetRightVector();
	glm::vec3 uv2 = t2.GetUpVector();

	glm::vec3 poi = {0.0f,0.0f,0.0f};
	float lowestoverlap = FLT_MAX;

	glm::vec3 cn = { 0.0f,0.0f,0.0f };
	float cnlowestoverlap = FLT_MAX;

	std::vector<glm::vec3> vectoraxes = {
		fv1,uv1,rv1
	};

	for (glm::vec3& axis : vectoraxes) {
		std::optional<AxisCollisionCN> b = TAxisCollidesTCN(worldvertices1, worldvertices2, axis);
		if (not b.has_value()) return {};

		if (b.value().overlap < lowestoverlap) {
			lowestoverlap = b.value().overlap;
			poi = b.value().POI;
		}

		if (b.value().overlap < cnlowestoverlap) {
			cnlowestoverlap = b.value().overlap;
			cn = axis * ((b.value().infront) ? 1.0f : -1.0f);
		}
	}

	std::vector<glm::vec3> axes = {
		fv2,uv2,rv2,
		glm::cross(fv1,fv2),
		glm::cross(fv1, rv2),
		glm::cross(fv1, uv2),
		glm::cross(rv1, fv2),
		glm::cross(rv1, rv2),
		glm::cross(rv1, uv2),
		glm::cross(uv1, fv2),
		glm::cross(uv1, rv2),
		glm::cross(uv1, uv2)
	};

	for (glm::vec3& axis : axes) {
		std::optional<std::pair<float, glm::vec3>> b = TAxisCollidesT(worldvertices1, worldvertices2, axis);
		if (not b.has_value()) return {};

		if (b.value().first < lowestoverlap) {
			lowestoverlap = b.value().first;
			poi = b.value().second;
		}
	}

	TInTInfo tr;
	tr.POI = poi;
	tr.CN = cn;

	//FINAL
	return tr;
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

	glm::vec3 normdir = glm::normalize(direction);

	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), normdir);

	glm::vec3 desiredUp = { 0.0f,1.0f,0.0f };
	glm::vec3 right = cross(normdir, desiredUp);
	desiredUp = cross(right, normdir);

	// Because of the 1rst rotation, the up is probably completely screwed up.
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	return rot2 * rot1;
}

glm::quat LookAt(glm::vec3 start, glm::vec3 end) {

	return LookAt(end - start);
}
