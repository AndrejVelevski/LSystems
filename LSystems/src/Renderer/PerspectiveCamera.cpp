#include "PerspectiveCamera.h"
#include "../Utils/Log.h"

PerspectiveCamera::PerspectiveCamera(float fov, float aspect)
{
	this->fov = fov;
	this->aspect = aspect;
	near = 0.001f;
	far = 1000000000000000000.0f;
}

glm::mat4 PerspectiveCamera::getView()
{
	getModel();
	return glm::lookAt(position, position + mFront, mUp);
}

glm::mat4 PerspectiveCamera::getProjection()
{
	return glm::perspective(glm::radians(fov), aspect, near, far);
}

