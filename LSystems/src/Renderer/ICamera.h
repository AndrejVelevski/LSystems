#pragma once

#include "Transform.h"

#include <glm/gtc/type_ptr.hpp>

class ICamera : public Transform
{
public:
	virtual glm::mat4 getView() = 0;
	virtual glm::mat4 getProjection() = 0;
};