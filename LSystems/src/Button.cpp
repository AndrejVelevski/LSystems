#include "Button.h"

Button::Button(float width, float height, float x, float y, const std::string& text, Font* font)
{
	mWidth = width;
	mHeight = height;
	mFont = font;

	std::vector<float>* vertices = new std::vector<float> {
		0, 0,
		mWidth, 0,
		0, mHeight,
		mWidth, mHeight
	};

	std::vector<uint32>* elements = new std::vector<uint32>{
		0, 2, 1,
		1, 2, 3
	};

	mMesh = new Mesh(new Shader("res/shaders/guiButton.vert", "res/shaders/guiButton.frag"), vertices, elements);
	mMesh->setAttribute2f("aPosition", 2, 0);
	mMesh->position.x = x;
	mMesh->position.y = y;

	setText(text);
	setColor({ 1, 1, 1 });
}

Button::~Button()
{
	delete mMesh;
	delete mLabel;
}

std::string Button::getText()
{
	return mLabel->getText();
}

void Button::setText(const std::string& text)
{
	delete mLabel;
	mLabel = new Label(mWidth, mHeight, mMesh->position.x, mMesh->position.y, text, mFont);
}

void Button::setColor(glm::vec3 color)
{
	mMesh->setUniform3f("uColor", color);
}

bool Button::contains(float x, float y)
{
	return x >= mMesh->position.x && y >= mMesh->position.y && x < mMesh->position.x + mWidth && y < mMesh->position.y + mHeight;
}

void Button::draw(ICamera* camera)
{
	mMesh->draw(camera);
	mLabel->draw(camera);
}
