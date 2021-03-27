#include "Shader.h"

#include <glad\glad.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../Utils/Log.h"

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