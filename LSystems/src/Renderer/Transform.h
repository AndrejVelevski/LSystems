#pragma once

#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform();
	Transform(const glm::vec3& position);
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 getModel() const;

private:
};

