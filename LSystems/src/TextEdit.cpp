#include "TextEdit.h"
#include "Utils/Log.h"

TextEdit::TextEdit(float width, float height, float x, float y, Font* font)
{
	mWidth = width;
	mHeight = height;
	mFont = font;

	{
		std::vector<float>* vertices = new std::vector<float>{
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
	}
	
	{
		std::vector<float>* vertices = new std::vector<float>{
			0, 0,
			0, (float)(font->fontSize() + font->size('p').y - font->bearing('p').y)
		};

		std::vector<uint32>* elements = new std::vector<uint32>{
			0, 1
		};

		mMeshCursor = new Mesh(new Shader("res/shaders/guiButton.vert", "res/shaders/guiButton.frag"), vertices, elements);
		mMeshCursor->setAttribute2f("aPosition", 2, 0);
	}

	mPadding = mFont->fontSize() / 2;

	setText("");

	setColor({ 1, 1, 1 });

	setPosition(x, y);
}

TextEdit::~TextEdit()
{
	delete mMesh;
	delete mMeshCursor;
	delete mLabel;
}

void TextEdit::setColor(glm::vec3 color)
{
	mMesh->setUniform3f("uColor", color);
}

bool TextEdit::contains(float x, float y)
{
	return x >= mMesh->position.x && y >= mMesh->position.y && x < mMesh->position.x + mWidth && y < mMesh->position.y + mHeight;
}

glm::vec2 TextEdit::getPosition()
{
	return { mMesh->position.x, mMesh->position.y };
}

void TextEdit::setPosition(float x, float y)
{
	mMesh->position.x = x;
	mMesh->position.y = y;

	mLabel->setPosition(x + mPadding, y);
}

int32 TextEdit::getCursorPosition()
{
	return mCursorPosition;
}

void TextEdit::setCursorPosition(int32 position)
{
	mCursorPosition = position;

	if (mCursorPosition < 0)
		mCursorPosition = 0;
	if (mCursorPosition >= text().size()+1)
		mCursorPosition = text().size();
}

void TextEdit::setCursorPosition(float mousex, float mousey)
{
	mousex -= mMesh->position.x;
	mousey -= mMesh->position.y;

	std::string text = mLabel->getText();
	for (uint32 i = 0; i < text.size(); ++i)
	{
		glm::vec2 cp1 = mLabel->characterPosition(i);
		glm::vec2 cp2 = mLabel->characterPosition(i + 1);
		if (cp1.x+mPadding+((cp2.x - cp1.x) / 2) > mousex)
		{
			setCursorPosition(i);
			return;
		}
	}

	setCursorPosition(text.size());
}

void TextEdit::addChar(int8 chr)
{
	std::string text = mLabel->getText();
	text.insert(text.begin()+mCursorPosition, chr);
	setText(text);
}

void TextEdit::removeChar()
{
	if (mCursorPosition == 0)
		return;
	std::string text = mLabel->getText();
	text.replace(text.begin() + mCursorPosition-1, text.begin() + mCursorPosition, "");
	setText(text);
}

std::string TextEdit::text()
{
	return mLabel->getText();
}

void TextEdit::setText(const std::string& text)
{
	if (mLabel != nullptr)
		mCursorPosition += text.size() - mLabel->getText().size();
	else
		mCursorPosition = 0;

	delete mLabel;

	mLabel = new Label(mWidth, mHeight, mMesh->position.x + mPadding, mMesh->position.y, text, mFont, Label::HAlign::LEFT);
}

void TextEdit::draw(ICamera* camera, bool drawCursor)
{
	mMesh->draw(camera);
	mLabel->draw(camera);

	if (drawCursor)
	{
		glm::vec2 charPos = mLabel->characterPosition(mCursorPosition);
		mMeshCursor->position.x = mMesh->position.x + charPos.x + mPadding;
		mMeshCursor->position.y = mMesh->position.y + charPos.y;
		mMeshCursor->draw(camera, Mesh::LINES);
	}
}

