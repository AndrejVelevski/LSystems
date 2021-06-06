#include "MainWindow.h"

int main(int argc, char** argv)
{
	MainWindow window(1060, 800, "L-Systems");
	window.exec();
	window.waitForFinished();

    return 0;
}