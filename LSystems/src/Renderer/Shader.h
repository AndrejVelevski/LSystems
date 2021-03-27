#pragma once

#include "../Utils/Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	uint32 getId();

	void bind() const;
	void unbind() const;

private:
	static uint32 compileShader(uint32 type, const std::string& source);
	static uint32 linkProgram(uint32 vertexShader, uint32 fragmentShader);

	uint32 mId;
};
