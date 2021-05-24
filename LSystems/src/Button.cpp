#include "Button.h"

Button::Button(float width, float height, float x, float y, const std::string& text, Font* font)
{
	mWidth = width;
	mHeight = height;
	mText = text;
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

	mLabel = new Label(width, height, x, y, text, font);

	mMesh = new Mesh(new Shader("res/shaders/guiButton.vert", "res/shaders/guiButton.frag"), vertices, elements);
	mMesh->setAttribute2f("aPosition", 2, 0);

	setColor({ 1, 1, 1 });

	mMesh->position.x = x;
	mMesh->position.y = y;
}

Button::~Button()
{
	delete mMesh;
	delete mLabel;
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
