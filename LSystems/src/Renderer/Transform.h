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

	glm::mat4 getModel();
	glm::vec3 right() const;
	glm::vec3 up() const;
	glm::vec3 front() const;

protected:
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mFront;
};

