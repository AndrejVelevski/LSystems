#pragma once

#include "ICamera.h"
#include "Transform.h"

class PerspectiveCamera : public Transform, public ICamera
{

public:
	PerspectiveCamera(float fov, float aspect);

	glm::mat4 getView() override;
	glm::mat4 getProjection() override;

	float fov;
	float aspect;
	float near;
	float far;
};

