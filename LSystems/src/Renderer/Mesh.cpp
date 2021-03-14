#include "Mesh.h"

#include <glad/glad.h>
#include "../Utils/Log.h"

Mesh::Mesh()
{
	mVAO = 0;
	mVBO = 0;
	mEBO = 0;
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32>& elements) :
	mVertices(vertices),
	mElements(elements)
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), &mVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mElements.size() * sizeof(uint32), &mElements[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::bind() const
{
	glBindVertexArray(mVAO);
}

void Mesh::unbind() const
{
	glBindVertexArray(0);
}

void Mesh::update(float delta)
{

}

void Mesh::draw(const Shader& shader) const
{
	glBindVertexArray(mVAO);
	shader.bind();
	glDrawElements(GL_TRIANGLES, mElements.size(), GL_UNSIGNED_INT, nullptr);
}
