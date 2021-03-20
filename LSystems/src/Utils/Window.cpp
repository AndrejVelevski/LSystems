#include "Window.h"

#include "Log.h"
#include "../Scenes/TestScene.h"

#include <mutex>

static std::mutex mutex;
static bool loaded = false;

//public
Window::Window(uint16 width, uint16 height, const char* title)
{
    glfwInit();

    mWidth = width;
    mHeight = height;
    mTitle = title;
    mThread = nullptr;
    mWindow = nullptr;
    mScene = nullptr;
}

void Window::exec()
{
    if (mThread != nullptr)
    {
        Log::warning("Window \"%s\" is already running", mTitle);
        return;
    }
    
    mThread = new std::thread(&Window::mfExec, this);
}

void Window::waitForFinished()
{
    if (mThread == nullptr)
    {
        Log::warning("Window \"%s\" is not running", mTitle);
        return;
    }

    mThread->join();
    delete mThread;
    mThread = nullptr;

    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
}

void Window::close()
{
    if (mThread == nullptr)
    {
        Log::warning("Window \"%s\" is not running", mTitle);
        return;
    }

    if (mWindow != nullptr)
        glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    waitForFinished();
}

//private
void Window::mfExec()
{
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);

    mutex.lock();
    gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
    mutex.unlock();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Log::success("(%s): %s", mTitle.c_str(), glGetString(GL_VERSION));
    mScene = new TestScene; //TODO: refactor

    float delta = 0;

    while (true)
    {
		float time = glfwGetTime();

		if (glfwWindowShouldClose(mWindow))
		{
			break;
		}

        if (mScene != nullptr)
        {
            mScene->update(delta);
            mScene->draw();
        }

        glfwSwapBuffers(mWindow);

        glfwPollEvents();

        delta = glfwGetTime() - time;
    }
}
