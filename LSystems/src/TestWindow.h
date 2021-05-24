#pragma once

#include "Utils/Window.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture.h"
#include "Renderer/Font.h"

class TestWindow : public Window
{
public:
	TestWindow(uint16 width, uint16 height, const std::string& title);

private:
	virtual void setup() override;
	virtual void update(float delta) override;
	virtual void draw() override;

	OrthographicCamera* mCamera;
	Mesh* mMesh;
	Texture* mTexture;
	Font* mFont;
	Mesh* mTextMesh;
};

