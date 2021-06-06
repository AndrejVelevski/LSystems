#pragma once

#include "ICamera.h"
#include "Transform.h"

class OrthographicCamera : public ICamera
{

public:
	OrthographicCamera(float left, float right, float top, float bottom);

	glm::mat4 getView() override;
	glm::mat4 getProjection() override;

	glm::vec3 front;
	glm::vec3 up;

	float left;
	float right;
	float top;
	float bottom;
	float near;
	float far;
};

