#include "Utils/Window.h"

int main(void)
{
	Window window1(640, 480, "L-Systems 1");
	Window window2(640, 480, "L-Systems 2");
	window1.exec();
	window2.exec();
	window1.waitForFinished();
	window2.waitForFinished();
    return 0;
}