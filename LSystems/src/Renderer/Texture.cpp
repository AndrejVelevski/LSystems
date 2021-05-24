#include "Texture.h"

#include "../Utils/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Texture::Texture(const std::string& source)
{
	glGenTextures(1, &mId);

	int32 width;
	int32 height;
	int32 channels;

	uint8* data = stbi_load(source.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	mWidth = width;
	mHeight = height;

	if (data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, mId);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		Log::error("Failed to load image: %s", source.c_str());
	}
}

Texture::Texture(int32 width, int32 height, uint8* data, int32 format)
{
	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mId);

	if (data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, mId);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (format == GL_RED)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] data;
	}
	else
	{
		Log::error("Failed to load data: data is nullptr");
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId);
}

void Texture::bind(uint8 slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mId);
}

int32 Texture::width() const
{
	return mWidth;
}

int32 Texture::height() const
{
	return mHeight;
}
