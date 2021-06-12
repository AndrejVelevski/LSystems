#pragma once

#include <string>
#include <vector>

#include "Renderer/ICamera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Font.h"

class Label
{
public:
	enum class HAlign {
		LEFT, CENTER, RIGHT
	};

	enum class VAlign {
		TOP, CENTER, BOTTOM
	};

	Label(float width, float height, float x, float y, const std::string& text, Font* font, HAlign hAlign=HAlign::CENTER, VAlign vAlign=VAlign::CENTER);
	~Label();

	std::string getText();
	void setColor(glm::vec3 color);
	glm::vec2 getPosition();
	void setPosition(float x, float y);
	float textWidth();
	float textHeight();
	glm::vec2 characterPosition(uint32 index);

	void draw(ICamera* camera);

private:
	std::string mText;
	std::vector<glm::vec2> mCharacterPositions;
	Mesh* mMesh;
	HAlign mHAlign;
	VAlign mVAlign;

	float mWidth;
	float mHeight;
	float mTextWidth;
	float mTextHeight;
};

