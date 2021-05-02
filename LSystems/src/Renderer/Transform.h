#pragma once

#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform();
	Transform(const Transform& transform);
	Transform& operator=(const Transform& transform);

	Transform(const glm::vec3& position);
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 getModel();

	glm::vec3 left() const;
	glm::vec3 right() const;
	glm::vec3 front() const;
	glm::vec3 back() const;
	glm::vec3 up() const;
	glm::vec3 down() const;

protected:
	glm::vec3 mLeft;
	glm::vec3 mUp;
	glm::vec3 mFront;
};

