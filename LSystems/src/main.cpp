#include "Utils/Log.h"
#include "Utils/Window.h"

int main(void)
{
    Window window(640, 480, "LSystems");
    Window window2(300, 300, "LSystems2");
    Window window3(400, 100, "LSystems3");

    window.exec();
    //window2.exec();
    //window3.exec();

    window.waitForFinished();
    //window2.waitForFinished();
    //window3.waitForFinished();
    return 0;
}