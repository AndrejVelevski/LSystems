#pragma once

#include "../Utils/Types.h"
#include "Texture.h"

#include <glm/vec2.hpp>

#include <unordered_map>
#include <string>

class Font
{
    struct Character
    {
        glm::ivec2 size;
        glm::ivec2 bearing;
        glm::vec2 texCoordsTL;
        glm::vec2 texCoordsTR;
        glm::vec2 texCoordsBL;
        glm::vec2 texCoordsBR;
        uint32 advance;
    };

public:
	Font(const std::string& source, uint32 size);

    glm::ivec2 size(int8 character);
    glm::ivec2 bearing(int8 character);
    glm::vec2 texCoordsTL(int8 character);
    glm::vec2 texCoordsTR(int8 character);
    glm::vec2 texCoordsBL(int8 character);
    glm::vec2 texCoordsBR(int8 character);
    uint32 advance(int8 character);
    Texture* texture();
    uint32 fontSize();

private:
    Character mCharacters[128];
    Texture* mTexture;
    uint32 mSize;
};

