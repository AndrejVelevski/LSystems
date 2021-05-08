#include "MainWindow.h"
#include "TestWindow.h"

int main(int argc, char** argv)
{
	int numWindows = 1;
	int width = 1024;
	int height = 768;
	MainWindow** windows = new MainWindow*[numWindows];

    /*TestWindow* w = new TestWindow(width, height, "Test");
    w->exec();
	w->waitForFinished();*/

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i] = new MainWindow(width, height, "L-Systems " + std::to_string(i+1));
	}

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i]->exec();
	}

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i]->waitForFinished();
	}

	for (int i = 0; i < numWindows; ++i)
	{
		delete windows[i];
	}

	delete[] windows;

    return 0;
}