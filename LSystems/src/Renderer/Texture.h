#pragma once

#include "../Utils/Types.h"

#include <glad\glad.h>

#include <string>

class Texture
{
public:
	Texture(const std::string& source);
	Texture(int32 width, int32 height, uint8* data, int32 format=GL_RGB);
	~Texture();

	void bind(uint8 slot=0) const;
	int32 width() const;
	int32 height() const;

private:
	uint32 mId;
	int32 mWidth;
	int32 mHeight;
};

