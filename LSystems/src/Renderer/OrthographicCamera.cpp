#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
{
	front = { 0, 0, -1 };
	up = { 0, 1, 0 };

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	near = -1000000000000.0;
	far = 1000000000000.0;
}

glm::mat4 OrthographicCamera::getView() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 OrthographicCamera::getView(const glm::vec3 target) const
{
	return glm::lookAt(position, target, up);
}

glm::mat4 OrthographicCamera::getProjection() const
{
	return glm::ortho(left, right, bottom, top, near, far);
}
