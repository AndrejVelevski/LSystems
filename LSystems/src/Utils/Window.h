#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>

#include "Types.h"

class Window
{
public:
	Window(uint16 width, uint16 height, const char* title);
	void exec();
	void waitForFinished();
	void close();

private:
	void mfExec();

	uint16 mWidth;
	uint16 mHeight;
	std::string mTitle;
	GLFWwindow* mWindow;
	std::thread* mThread;
};

