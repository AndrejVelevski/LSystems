#pragma once

#include <glm/gtc/type_ptr.hpp>

class ICamera
{
public:
	virtual glm::mat4 getView() const = 0;
	virtual glm::mat4 getProjection() const = 0;
};