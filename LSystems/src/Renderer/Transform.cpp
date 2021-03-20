#include "Transform.h"

Transform::Transform()
{
	this->position = { 0, 0, 0 };
	this->rotation = { 0, 0, 0 };
	this->scale = { 1, 1, 1 };
}

Transform::Transform(const glm::vec3& position)
{
	this->position = position;
	this->rotation = { 0, 0, 0 };
	this->scale = { 0, 0, 0 };
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::mat4 Transform::getModel() const
{
	glm::mat4 transform = glm::mat4(1.0);

	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	transform = glm::translate(transform, position);

	return transform;
}
