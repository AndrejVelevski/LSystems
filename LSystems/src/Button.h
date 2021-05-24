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

	void setColor(glm::vec3 color);
	bool contains(float x, float y);
	
	void draw(ICamera* camera);


private:
	std::string mText;
	Mesh* mMesh;
	Label* mLabel;

	float mWidth;
	float mHeight;
};

