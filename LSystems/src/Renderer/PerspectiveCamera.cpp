#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
{
	front = { 0, 0, -1 };
	up = { 0, 1, 0 };

	fov = 70;
	aspect = 1;
	near = 0.1;
	far = 1000;
}

glm::mat4 PerspectiveCamera::getView() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 PerspectiveCamera::getView(const glm::vec3 target) const
{
	return glm::lookAt(position, target, up);
}

glm::mat4 PerspectiveCamera::getProjection() const
{
	return glm::perspective(glm::radians(fov), aspect, near, far);
}
