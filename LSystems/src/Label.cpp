#include "Label.h"

#include "Utils/Log.h"

Label::Label(float width, float height, float x, float y, const std::string& text, Font* font, HAlign hAlign, VAlign vAlign)
{
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;

	mText = text;
	mHAlign = hAlign;
	mVAlign = vAlign;

	float xpos = 0;
	float ypos = 0;
	int element = 0;

	mCharacterPositions.push_back({ xpos, ypos });

	for (char c : text)
	{
		if (c == '\n')
		{
			xpos = 0;
			ypos += font->fontSize();
			continue;
		}

		glm::ivec2 bearing = font->bearing(c);
		glm::ivec2 size = font->size(c);
		glm::vec2 tl = font->texCoordsTL(c);
		glm::vec2 tr = font->texCoordsTR(c);
		glm::vec2 bl = font->texCoordsBL(c);
		glm::vec2 br = font->texCoordsBR(c);

		float x = xpos + bearing.x;
		float y = ypos + font->fontSize() - bearing.y;
		float w = size.x;
		float h = size.y;

		vertices->push_back(x);
		vertices->push_back(y);
		vertices->push_back(tl.x);
		vertices->push_back(tl.y);

		vertices->push_back(x + w);
		vertices->push_back(y);
		vertices->push_back(tr.x);
		vertices->push_back(tr.y);

		vertices->push_back(x);
		vertices->push_back(y + h);
		vertices->push_back(bl.x);
		vertices->push_back(bl.y);

		vertices->push_back(x + w);
		vertices->push_back(y + h);
		vertices->push_back(br.x);
		vertices->push_back(br.y);

		elements->push_back(element + 0);
		elements->push_back(element + 2);
		elements->push_back(element + 1);

		elements->push_back(element + 1);
		elements->push_back(element + 2);
		elements->push_back(element + 3);

		xpos += (font->advance(c) >> 6);
		element += 4;
		mCharacterPositions.push_back({ xpos, ypos });
	}

	mWidth = width;
	mHeight = height;
	mTextWidth = xpos;
	mTextHeight = ypos+font->fontSize();

	mMesh = new Mesh(new Shader("res/shaders/guiLabel.vert", "res/shaders/guiLabel.frag"), vertices, elements);
	mMesh->setAttribute2f("aPosition", 4, 0);
	mMesh->setAttribute2f("aTexCoords", 4, 2);

	setColor({ 0, 0, 0 });

	setPosition(x, y);
}

Label::~Label()
{
	delete mMesh;
}

void Label::setColor(glm::vec3 color)
{
	mMesh->setUniform3f("uColor", color);
}

std::string Label::getText()
{
	return mText;
}

void Label::setPosition(float x, float y)
{
	switch (mHAlign)
	{
		case HAlign::LEFT: mMesh->position.x = x; break;
		case HAlign::CENTER: mMesh->position.x = x + (int32)(mWidth - mTextWidth) / 2; break;
		case HAlign::RIGHT: mMesh->position.x = x + mWidth - mTextWidth;
	}

	switch (mVAlign)
	{
		case VAlign::TOP: mMesh->position.y = y; break;
		case VAlign::CENTER: mMesh->position.y = y + (int32)(mHeight - mTextHeight) / 2; break;
		case VAlign::BOTTOM: mMesh->position.y = y + mHeight - mTextHeight;
	}
}

glm::vec2 Label::getPosition()
{
	return { mMesh->position.x, mMesh->position.y };
}

float Label::textWidth()
{
	return mTextWidth;
}

float Label::textHeight()
{
	return mTextHeight;
}

void Label::draw(ICamera* camera)
{
	mMesh->draw(camera);
}

glm::vec2 Label::characterPosition(uint32 index)
{
	glm::vec2 pos;
	if (index < mCharacterPositions.size())
		pos = mCharacterPositions[index];
	else
		pos = { 0, 0 };


	switch(mHAlign)
	{
		case HAlign::CENTER: pos.x += (int32)(mWidth - mTextWidth) / 2; break;
		case HAlign::RIGHT: pos.x += mWidth - mTextWidth; break;
	}

	switch(mVAlign)
	{
		case VAlign::CENTER: pos.y += (int32)(mHeight - mTextHeight) / 2; break;
		case VAlign::BOTTOM: pos.y + mHeight - mTextHeight; break;
	}

	return pos;
}
