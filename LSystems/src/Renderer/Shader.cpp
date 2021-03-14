#include "Shader.h"

#include <glad\glad.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../Utils/Log.h"

Shader::Shader()
{
	mId = 0;
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	std::stringstream vertex, fragment;

	std::ifstream vertexFile(vertexSource);
	std::ifstream fragmentFile(fragmentSource);

	if (vertexFile.is_open())
	{
		std::string line;

		while (std::getline(vertexFile, line))
		{
			vertex << line << std::endl;
		}
		vertexFile.close();
	}
	else
	{
		Log::error("Failed to open vertex source file: %s", vertexSource);
	}

	if (fragmentFile.is_open())
	{
		std::string line;

		while (std::getline(fragmentFile, line))
		{
			fragment << line << std::endl;
		}
		fragmentFile.close();
	}
	else
	{
		Log::error("Failed to open fragment source file: %s", fragmentSource);
	}

	uint32 vertexShader = compileShader(GL_VERTEX_SHADER, vertex.str().c_str());
	uint32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment.str().c_str());
	mId = linkProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(mId);
}

uint32 Shader::getId()
{
	return mId;
}

void Shader::setUniform1i(const std::string& uniform, int32 i)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform1i(iterator->second, i);
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform1i(location, i);
	}
}

void Shader::setUniform1f(const std::string& uniform, float f)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform1f(iterator->second, f);
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform1f(location, f);
	}
}

void Shader::setUniform2f(const std::string& uniform, const glm::vec2& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform2f(iterator->second, vec.x, vec.y);
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform2f(location, vec.x, vec.y);
	}
}

void Shader::setUniform3f(const std::string& uniform, const glm::vec3& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform3f(iterator->second, vec.x, vec.y, vec.z);
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform3f(location, vec.x, vec.y, vec.z);
	}
}

void Shader::setUniform4f(const std::string& uniform, const glm::vec4& vec)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniform4f(iterator->second, vec.x, vec.y, vec.z, vec.z);
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
}

void Shader::setUniformMatrix4fv(const std::string& uniform, const glm::mat4& mat)
{
	auto iterator = mUniforms.find(uniform);
	if (iterator != mUniforms.end())
	{
		glUniformMatrix4fv(iterator->second, 1, GL_FALSE, glm::value_ptr(mat));
	}
	else
	{
		int location = glGetUniformLocation(mId, uniform.c_str());
		mUniforms.insert(std::make_pair(uniform, location));
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void Shader::setAttribute2f(const std::string& attribute, uint32 vertexSizeInBytes, uint32 attributeOffsetInBytes)
{
	auto iterator = mAttributes.find(attribute);
	if (iterator != mAttributes.end())
	{
		glVertexAttribPointer(iterator->second, 2, GL_FLOAT, GL_FALSE, vertexSizeInBytes, (void*)attributeOffsetInBytes);
		glEnableVertexAttribArray(iterator->second);
	}
	else
	{
		int location = glGetAttribLocation(mId, attribute.c_str());
		mAttributes.insert(std::make_pair(attribute, location));
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, vertexSizeInBytes, (void*)attributeOffsetInBytes);
		glEnableVertexAttribArray(location);
	}
}

void Shader::bind() const
{
	glUseProgram(mId);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

uint32 Shader::compileShader(uint32 type, const std::string& source)
{
	const char* src = source.c_str();
	uint32 shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(shader, length, nullptr, message);

		std::string shaderType = "Unknown";
		switch (type)
		{
			case GL_VERTEX_SHADER: shaderType = "Vertex"; break;
			case GL_FRAGMENT_SHADER: shaderType = "Fragment"; break;
		}

		Log::error("Failed to compile %s shader:\n%s\n", shaderType, message);
		delete[] message;
	}

	return shader;
}

uint32 Shader::linkProgram(uint32 vertexShader, uint32 fragmentShader)
{
	uint32 program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetProgramInfoLog(program, length, nullptr, message);

		Log::error("Failed to link program:\n%s", message);
		delete[] message;
	}

	return program;
}