#pragma once

#include "Utils/Window.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

class MainWindow : public Window
{
public:
	MainWindow(uint16 width, uint16 height, const std::string& title);

private:
	void setup() override;
	void update(float delta) override;
	void draw() override;

	void keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods) const override;
	void mouseButtonsCallback(int32 button, int32 action, int32 mods) const override;
	void mousePositionCallback(double xpos, double ypos) const override;
	void mouseEnteredCallback(int32 entered) const override;

	Mesh* mMesh;
	PerspectiveCamera* mPCamera;
	OrthographicCamera* mOCamera;
};

