#include "MainWindow.h"

int main(int argc, char** argv)
{
	int numWindows = 20;
	int width = 200;
	int height = 200;
	MainWindow** windows = new MainWindow*[numWindows];

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