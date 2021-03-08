#include "Window.h"
#include "Log.h"
#include <mutex>

static std::mutex contextMutex;
static std::mutex eventMutex;
static std::mutex bufferMutex;

//public
Window::Window(uint16 width, uint16 height, const char* title)
{
    glfwInit();
    mWidth = width;
    mHeight = height;
    mTitle = title;
    mThread = nullptr;
    mWindow = nullptr;
}

void Window::exec()
{
    if (mThread != nullptr)
    {
        Log::warning("Window is already running");
        return;
    }
    
    mThread = new std::thread(&Window::mfExec, this);
}

void Window::waitForFinished()
{
    if (mThread == nullptr)
    {
        Log::warning("Window is not running");
        return;
    }

    mThread->join();
    delete mThread;
    mThread = nullptr;

    glfwDestroyWindow(mWindow);
    //delete mWindow;
    mWindow = nullptr;
}

// TODO: fix
void Window::close()
{
    if (mThread == nullptr)
    {
        Log::warning("Window is not running");
        return;
    }

    if (mWindow != nullptr)
        glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    waitForFinished();
}

//private
void Window::mfExec()
{
    contextMutex.lock();
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
    Log::success("(%s): %s", mTitle.c_str(), glGetString(GL_VERSION));
    contextMutex.unlock();

    while (true)
    {
        contextMutex.lock();
        glfwMakeContextCurrent(mWindow);

        if (glfwWindowShouldClose(mWindow))
        {
            contextMutex.unlock();
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        contextMutex.unlock();

        bufferMutex.lock();
        glfwSwapBuffers(mWindow);
        bufferMutex.unlock();

        eventMutex.lock();
        glfwPollEvents();
        eventMutex.unlock();
    }
}
