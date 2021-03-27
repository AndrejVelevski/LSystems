#include "Mesh.h"

#include <glad/glad.h>
#include "../Utils/Log.h"

Mesh::Mesh(Shader* shader, std::vector<float>* vertices, std::vector<uint32>* elements)
{
	mShader = shader;
	mVertices = vertices;
	mElements = elements;

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices->size() * sizeof(float), &mVertices->at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mElements->size() * sizeof(uint32), &mElements->at(0), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &mEBO);
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);

	delete mVertices;
	delete mElements;
	delete mShader;
}

void Mesh::updateData(uint32 index, uint32 count)
{
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(float), count * sizeof(float), &mVertices->at(index));
}

void Mesh::draw(ICamera* camera)
{
	glBindVertexArray(mVAO);
	mShader->bind();
	setUniformMatrix4fv("uModel", getModel());
	setUniformMatrix4fv("uView", camera->getView());
	setUniformMatrix4fv("uProjection", camera->getProjection());
	glDrawElements(GL_TRIANGLES, mElements->size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::setUniform1i(const std::string& uniform, int32 i)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform1i(iterator->second, i);
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform1i(location, i);
	}
}

void Mesh::setUniform1f(const std::string& uniform, float f)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform1f(iterator->second, f);
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform1f(location, f);
	}
}

void Mesh::setUniform2f(const std::string& uniform, const glm::vec2& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform2f(iterator->second, vec.x, vec.y);
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform2f(location, vec.x, vec.y);
	}
}

void Mesh::setUniform3f(const std::string& uniform, const glm::vec3& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform3f(iterator->second, vec.x, vec.y, vec.z);
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform3f(location, vec.x, vec.y, vec.z);
	}
}

void Mesh::setUniform4f(const std::string& uniform, const glm::vec4& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform4f(iterator->second, vec.x, vec.y, vec.z, vec.z);
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
}

void Mesh::setUniformMatrix4fv(const std::string& uniform, const glm::mat4& mat)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniformMatrix4fv(iterator->second, 1, GL_FALSE, glm::value_ptr(mat));
	}
	else
	{
		int32 location = glGetUniformLocation(mShader->getId(), uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void Mesh::setAttribute2f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset)
{
	glBindVertexArray(mVAO);
	auto iterator = mAttributes.find(attribute);
	if (iterator != mAttributes.end())
	{
		glVertexAttribPointer(iterator->second, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(attributeOffset * sizeof(float)));
		glEnableVertexAttribArray(iterator->second);
	}
	else
	{
		int32 location = glGetAttribLocation(mShader->getId(), attribute.c_str());
		mAttributes.insert(std::make_pair(attribute, location));
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(attributeOffset * sizeof(float)));
		glEnableVertexAttribArray(location);
	}
	glBindVertexArray(0);
}

void Mesh::setAttribute3f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset)
{
	glBindVertexArray(mVAO);
	auto iterator = mAttributes.find(attribute);
	if (iterator != mAttributes.end())
	{
		glVertexAttribPointer(iterator->second, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(attributeOffset * sizeof(float)));
		glEnableVertexAttribArray(iterator->second);
	}
	else
	{
		int location = glGetAttribLocation(mShader->getId(), attribute.c_str());
		mAttributes.insert(std::make_pair(attribute, location));
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(attributeOffset * sizeof(float)));
		glEnableVertexAttribArray(location);
	}
	glBindVertexArray(0);
}
