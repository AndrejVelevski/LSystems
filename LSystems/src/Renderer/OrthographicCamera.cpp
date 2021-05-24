#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float left, float top, float right, float bottom)
{
	front = { 0, 0, -1 };
	up = { 0, 1, 0 };

	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	near = -1000000000000000000.0f;
	far = 1000000000000000000.0f;
}

glm::mat4 OrthographicCamera::getView()
{
	getModel();
	return glm::lookAt(position, position + front, up);
}

glm::mat4 OrthographicCamera::getProjection()
{
	return glm::ortho(left, right, bottom, top, near, far);
}
