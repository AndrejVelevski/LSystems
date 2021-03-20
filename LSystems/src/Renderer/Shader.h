#pragma once

#include "../Utils/Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	uint32 getId();
	void setUniform1i(const std::string& uniform, int32 i);
	void setUniform1f(const std::string& uniform, float f);
	void setUniform2f(const std::string& uniform, const glm::vec2& vec);
	void setUniform3f(const std::string& uniform, const glm::vec3& vec);
	void setUniform4f(const std::string& uniform, const glm::vec4& vec);
	void setUniformMatrix4fv(const std::string& uniform, const glm::mat4& mat);

	void setAttribute2f(const std::string& attribute, uint32 vertexSizeInBytes, uint32 attributeOffsetInBytes);
	void setAttribute3f(const std::string& attribute, uint32 vertexSizeInBytes, uint32 attributeOffsetInBytes);

	void bind() const;
	void unbind() const;

private:
	static uint32 compileShader(uint32 type, const std::string& source);
	static uint32 linkProgram(uint32 vertexShader, uint32 fragmentShader);

	uint32 mId;
	std::unordered_map<std::string, uint32> mUniforms;
	std::unordered_map<std::string, uint32> mAttributes;
};
