#pragma once

#include "Utils/Window.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/OrthographicCamera.h"


class TestWindow : public Window
{
public:
	TestWindow(uint16 width, uint16 height, const std::string& title);

private:
	virtual void setup() override;
	virtual void update(float delta) override;
	virtual void draw() override;

	Mesh* generateDavidStar(Shader* shader);

	OrthographicCamera* mCamera;
	Mesh* mMesh;
};

