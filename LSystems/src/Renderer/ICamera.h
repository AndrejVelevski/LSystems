#pragma once

#include <glm/gtc/type_ptr.hpp>

class ICamera
{
public:
	virtual glm::mat4 getView() = 0;
	virtual glm::mat4 getProjection() = 0;
};