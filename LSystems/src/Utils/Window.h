#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>

#include "Types.h"
#include "Log.h"

class Window
{
public:
	Window(uint16 width, uint16 height, const std::string& title);
	void exec();
	void waitForFinished();
	void close();

protected:
	void mfExec();
	virtual void setup() = 0;
	virtual void update(float delta) = 0;
	virtual void draw() = 0;

	virtual void keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods) {};
	virtual void mouseButtonsCallback(int32 button, int32 action, int32 mods) {};
	virtual void mousePositionCallback(double xpos, double ypos) {};
	virtual void mouseEnteredCallback(int32 entered) {};

	uint16 mWidth;
	uint16 mHeight;
	std::string mTitle;
	GLFWwindow* mWindow;
	std::thread* mThread;
};

