#include "Mesh.h"

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

	if (mVertices != nullptr && mVertices->size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices->size() * sizeof(float), &mVertices->at(0), GL_STATIC_DRAW);
	}

	if (mElements != nullptr && mElements->size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mElements->size() * sizeof(uint32), &mElements->at(0), GL_STATIC_DRAW);
	}
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

void Mesh::draw(ICamera* camera, Mode mode)
{
	glBindVertexArray(mVAO);
	mShader->bind();
	setUniformMatrix4fv("uModel", getModel());
	setUniformMatrix4fv("uView", camera->getView());
	setUniformMatrix4fv("uProjection", camera->getProjection());

	if (mElements != nullptr)
		glDrawElements(mode, mElements->size(), GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(mode, 0, mVertices->size()/2);
}

void Mesh::setUniform1i(const std::string& uniform, int32 i)
{
	mShader->bind();
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
	mShader->bind();
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
	mShader->bind();
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
	mShader->bind();
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
	mShader->bind();
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
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
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
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
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
}

void Mesh::generateCylinder(float bottomRadius, float topRadius, float height, std::vector<glm::vec3>& vertices, std::vector<uint32>& elements, uint32 precision)
{
	vertices.push_back({ 0, 0, 0 });
	for (int i = 0; i < precision; ++i)
	{
		float angle = glm::radians(i * 360.0 / precision);
		float x = bottomRadius * cos(angle);
		float y = 0;
		float z = bottomRadius * sin(angle);

		vertices.push_back({ x, y, z });

		if (i != precision - 1)
		{
			elements.push_back(i + 2);
			elements.push_back(i + 1);
			elements.push_back(0);
		}
		else
		{
			elements.push_back(1);
			elements.push_back(i + 1);
			elements.push_back(0);
		}
	}

	vertices.push_back({ 0, height, 0 });
	for (int i = 0; i < precision; ++i)
	{
		float angle = glm::radians(i * 360.0 / precision);
		float x = topRadius * cos(angle);
		float y = height;
		float z = topRadius * sin(angle);

		vertices.push_back({ x, y, z });

		if (i != precision - 1)
		{
			elements.push_back(precision + 1);
			elements.push_back(precision + i + 2);
			elements.push_back(precision + i + 3);
		}
		else
		{
			elements.push_back(precision + 1);
			elements.push_back(precision + i + 2);
			elements.push_back(precision + 2);
		}
	}

	for (int i = 0; i < precision; ++i)
	{
		if (i != precision - 1)
		{
			elements.push_back(i + 1);
			elements.push_back(i + 2);
			elements.push_back(i + precision + 2);
			elements.push_back(i + 2);
			elements.push_back(i + precision + 3);
			elements.push_back(i + precision + 2);
		}
		else
		{
			elements.push_back(i + 1);
			elements.push_back(1);
			elements.push_back(i + precision + 2);
			elements.push_back(1);
			elements.push_back(precision + 2);
			elements.push_back(i + precision + 2);
		}
	}
}
