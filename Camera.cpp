#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) : width(width), height(height)
{
	t.TranslateTo(position);
}
