#pragma once

#include <string>

#include "Utils/Types.h"
#include "Renderer/ICamera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Font.h"

#include "Label.h"

class TextEdit
{
public:
	TextEdit(float width, float height, float x, float y, Font* font);
	~TextEdit();

	void setColor(glm::vec3 color);
	bool contains(float x, float y);

	glm::vec2 getPosition();
	void setPosition(float x, float y);

	int32 getCursorPosition();
	void setCursorPosition(int32 position);
	void setCursorPosition(float mousex, float mousey);

	void addChar(int8 chr);
	void removeChar();
	std::string text();
	void setText(const std::string& text);

	void draw(ICamera* camera, bool drawCursor=false);

private:
	Mesh* mMesh;
	Mesh* mMeshCursor;
	Label* mLabel;
	Font* mFont;

	int32 mCursorPosition;

	float mWidth;
	float mHeight;
	float mPadding;
};
