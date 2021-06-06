#pragma once

#include <string>

#include "Utils/Types.h"
#include "Renderer/ICamera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Font.h"

#include "Label.h"

class Button
{
public:
	Button(float width, float height, float x, float y, const std::string& text, Font* font);
	~Button();

	std::string text();
	void setText(const std::string& text);
	void setColor(glm::vec3 color);
	bool contains(float x, float y);
	
	void draw(ICamera* camera);


private:
	Mesh* mMesh;
	Label* mLabel;
	Font* mFont;

	float mWidth;
	float mHeight;
};

