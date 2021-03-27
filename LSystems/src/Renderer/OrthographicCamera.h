#pragma once

#include "ICamera.h"
#include "Transform.h"

class OrthographicCamera : public Transform, public ICamera
{

public:
	OrthographicCamera(float left, float right, float top, float bottom);

	glm::mat4 getView() const override;
	glm::mat4 getView(const glm::vec3 target) const;
	glm::mat4 getProjection() const override;

	glm::vec3 front;
	glm::vec3 up;

	float left;
	float right;
	float top;
	float bottom;
	float near;
	float far;
};

