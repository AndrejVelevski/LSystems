#pragma once

#include "Shader.h"
#include "Transform.h"
#include "ICamera.h"
#include "../Utils/Types.h"

#include <vector>

class Mesh : public Transform
{
public:
	Mesh(Shader* shader, std::vector<float>* vertices, std::vector<uint32>* elements);
	~Mesh();

	void updateData(uint32 index, uint32 count);
	void draw(ICamera* camera);

	void setUniform1i(const std::string& uniform, int32 i);
	void setUniform1f(const std::string& uniform, float f);
	void setUniform2f(const std::string& uniform, const glm::vec2& vec);
	void setUniform3f(const std::string& uniform, const glm::vec3& vec);
	void setUniform4f(const std::string& uniform, const glm::vec4& vec);
	void setUniformMatrix4fv(const std::string& uniform, const glm::mat4& mat);

	void setAttribute2f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset);
	void setAttribute3f(const std::string& attribute, uint32 vertexSize, uint32 attributeOffset);

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

