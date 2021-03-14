#include "Utils/Window.h"

int main(void)
{
    Window window(640, 480, "LSystems");
    window.exec();
    window.waitForFinished();
    return 0;
}