#pragma once

#include "ICamera.h"
#include "Transform.h"

class PerspectiveCamera : public Transform, public ICamera
{

public:
	PerspectiveCamera(float fov, float aspect);

	glm::mat4 getView() const override;
	glm::mat4 getView(const glm::vec3 target) const;
	glm::mat4 getProjection() const override;

	glm::vec3 front;
	glm::vec3 up;

	float fov;
	float aspect;
	float near;
	float far;
};

