#include "Transform.h"

Transform::Transform()
{
	this->position = { 0, 0, 0 };
	this->rotation = { 0, 0, 0 };
	this->scale = { 1, 1, 1 };

	getModel();
}

Transform::Transform(const glm::vec3& position)
{
	this->position = position;
	this->rotation = { 0, 0, 0 };
	this->scale = { 0, 0, 0 };

	getModel();
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	getModel();
}

glm::mat4 Transform::getModel()
{
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	translationMatrix = glm::translate(glm::mat4(1.0), position);
	rotationMatrix = glm::mat4(glm::quat(glm::radians(rotation)));
	scaleMatrix = glm::scale(glm::mat4(1.0), scale);

	mRight = -normalize(glm::vec3(rotationMatrix[0]));
	mUp = normalize(glm::vec3(rotationMatrix[1]));
	mFront = normalize(glm::vec3(rotationMatrix[2]));

	return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 Transform::right() const
{
	return mRight;
}

glm::vec3 Transform::up() const
{
	return mUp;
}

glm::vec3 Transform::front() const
{
	return mFront;
}
