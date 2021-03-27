#include "Window.h"

#include <mutex>

static std::mutex mutex;
static std::mutex mutex2;

Window::Window(uint16 width, uint16 height, const std::string& title)
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

void Window::mfExec()
{
	mutex.lock();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSetWindowUserPointer(mWindow, this);

    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
        Log::fatal("Window \"%s\" failed to initialize GLAD", mTitle);
    
    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
        ((Window*)glfwGetWindowUserPointer(window))->keyboardCallback(key, scancode, action, mods);
    });

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int32 button, int32 action, int32 mods) {
		((Window*)glfwGetWindowUserPointer(window))->mouseButtonsCallback(button, action, mods);
	});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos) {
		((Window*)glfwGetWindowUserPointer(window))->mousePositionCallback(xpos, ypos);
	});

	glfwSetCursorEnterCallback(mWindow, [](GLFWwindow* window, int32 entered) {
		((Window*)glfwGetWindowUserPointer(window))->mouseEnteredCallback(entered);
	});

	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int32 width, int32 height) {
        glViewport(0, 0, width, height);
        ((Window*)glfwGetWindowUserPointer(window))->mWidth = width;
        ((Window*)glfwGetWindowUserPointer(window))->mHeight = height;
	});
    mutex.unlock();

    setup();

	Log::success("(%s): %s", mTitle.c_str(), glGetString(GL_VERSION));

    float delta = 0;

    while (true)
    {
        //glfwWaitEvents();

		float time = glfwGetTime();

		if (glfwWindowShouldClose(mWindow))
		{
			break;
		}

        update(delta);
        draw();

        glfwSwapBuffers(mWindow);

        glfwPollEvents();

        delta = glfwGetTime() - time;
    }
}
