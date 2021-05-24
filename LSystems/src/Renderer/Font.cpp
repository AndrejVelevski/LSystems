#include "Font.h"

#include "../Utils/Log.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

Font::Font(const std::string& source, uint32 size)
{
    mSize = size;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        Log::error("Could not initialize FreeType library");
        return;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, source.c_str(), 0, &face))
    {
        Log::error("Failed to load font: %s", source.c_str());
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    int32 width = 0;
    int32 height = size;

    for (uint8 c = 0; c < 128; c++)
    {
        //FT_LOAD_BITMAP_METRICS_ONLY
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Log::error("Failed to load Glyph: %c", c);
            continue;
        }

        Character character = {
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            glm::vec2(0, 0),
            glm::vec2(0, 0),
            glm::vec2(0, 0),
            glm::vec2(0, 0),
            face->glyph->advance.x
        };

        width += character.size.x;

        mCharacters[c] = character;
    }

    uint8* data = new uint8[width * height];

    uint32 position = 0;

    for (uint8 c = 0; c < 128; c++)
    {
        Character character = mCharacters[c];

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Log::error("Failed to load Glyph: %c", c);
            continue;
        }

        for (uint32 x = 0; x < character.size.x; ++x)
        {
            for (uint32 y = 0; y < character.size.y; ++y)
            {
                uint32 charidx = x * character.size.y + y;
                uint32 texidx = (charidx / character.size.x) * width + (charidx % character.size.x) + position;
                data[texidx] = face->glyph->bitmap.buffer[charidx];
            }
        }

        mCharacters[c].texCoordsTL = { position / (float)width, 0 };
        mCharacters[c].texCoordsTR = { (position + character.size.x) / (float)width, 0 };
        mCharacters[c].texCoordsBL = { position / (float)width, character.size.y / (float)height };
        mCharacters[c].texCoordsBR = { (position + character.size.x) / (float)width, character.size.y / (float)height };

        position += character.size.x;
    }

    mTexture = new Texture(width, height, data, GL_RED);
}

glm::ivec2 Font::size(int8 character)
{
    return mCharacters[character].size;
}

glm::ivec2 Font::bearing(int8 character)
{
    return mCharacters[character].bearing;
}

glm::vec2 Font::texCoordsTL(int8 character)
{
    return mCharacters[character].texCoordsTL;
}

glm::vec2 Font::texCoordsTR(int8 character)
{
    return mCharacters[character].texCoordsTR;
}

glm::vec2 Font::texCoordsBL(int8 character)
{
    return mCharacters[character].texCoordsBL;
}

glm::vec2 Font::texCoordsBR(int8 character)
{
    return mCharacters[character].texCoordsBR;
}

uint32 Font::advance(int8 character)
{
    return mCharacters[character].advance;
}

Texture* Font::texture()
{
    return mTexture;
}

uint32 Font::fontSize()
{
    return mSize;
}
