#pragma once

#include "Shader.h"
#include "Transform.h"
#include "ICamera.h"
#include "../Utils/Types.h"

#include <glad/glad.h>

#include <vector>

class Mesh : public Transform
{
public:
	static enum Mode
	{
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PATCHES = GL_PATCHES
	};

public:
	Mesh(Shader* shader, std::vector<float>* vertices, std::vector<uint32>* elements);
	~Mesh();

	void updateData(uint32 index, uint32 count);
	void draw(ICamera* camera, Mode mode=Mode::TRIANGLES);

	void setUniform1i(const std::string& uniform, int32 i);
	void setUniform1f(const std::string& uniform, float f);
	void setUniform2f(const std::string& uniform, const glm::vec2& vec);
	void setUniform3f(const std::string& uniform, const glm::vec3& vec);
	void setUniform4f(const std::string& uniform, const glm::vec4& vec);
	void setUniformMatrix4fv(const std::string& uniform, const glm::mat4& mat);
	void setAttribute2f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset);
	void setAttribute3f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset);

	static void generateCylinder(float bottomRadius, float topRadius, float height, std::vector<glm::vec3>& vertices, std::vector<uint32>& elements, uint32 precision=20);

private:
	uint32 mVAO;
	uint32 mVBO;
	uint32 mEBO;

	std::unordered_map<std::string, uint32> mUniforms;
	std::unordered_map<std::string, uint32> mAttributes;

	Shader* mShader = nullptr;
	std::vector<float>* mVertices = nullptr;
	std::vector<uint32>* mElements = nullptr;
};

