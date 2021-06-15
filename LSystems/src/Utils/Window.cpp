#include "Window.h"

#include <mutex>

static std::mutex mutex;

Window::Window(uint16 width, uint16 height, const std::string& title)
{
    glfwInit();

    mWidth = width;
    mHeight = height;
    mTitle = title;
    mThread = nullptr;
    mWindow = nullptr;

    mCursorArrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    mCursorIbeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    mCursorCrosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    mCursorHand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    mCursorHresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    mCursorVresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
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

    glfwDestroyCursor(mCursorArrow);
    glfwDestroyCursor(mCursorIbeam);
    glfwDestroyCursor(mCursorCrosshair);
    glfwDestroyCursor(mCursorHand);
    glfwDestroyCursor(mCursorHresize);
    glfwDestroyCursor(mCursorVresize);

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
    //waitForFinished();
}

uint16 Window::width()
{
    return mWidth;
}

uint16 Window::height()
{
    return mHeight;
}

void Window::setCursor(Cursor cursor)
{
    switch (cursor)
    {
        case ARROW: glfwSetCursor(mWindow, mCursorArrow); break;
        case IBEAM: glfwSetCursor(mWindow, mCursorIbeam); break;
        case CROSSHAIR: glfwSetCursor(mWindow, mCursorCrosshair); break;
        case HAND: glfwSetCursor(mWindow, mCursorHand); break;
        case HRESIZE: glfwSetCursor(mWindow, mCursorHresize); break;
        case VRESIZE: glfwSetCursor(mWindow, mCursorVresize); break;
    }
}

void Window::setCursorEnabled(bool enabled)
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::setCursorPosition(double x, double y)
{
    glfwSetCursorPos(mWindow, x, y);
}

void Window::mfExec()
{
	mutex.lock();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSetWindowUserPointer(mWindow, this);

    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
        Log::fatal("Window \"%s\" failed to initialize GLAD", mTitle);

    glfwSetCharCallback(mWindow, [](GLFWwindow* window, uint32 chr) {
        ((Window*)glfwGetWindowUserPointer(window))->characterCallback(chr);
    });
    
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
        ((Window*)glfwGetWindowUserPointer(window))->mWidth = width;
        ((Window*)glfwGetWindowUserPointer(window))->mHeight = height;
	});

	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int32 width, int32 height) {
		glViewport(0, 0, width, height);
	});
    mutex.unlock();

    setup();

	Log::success("(%s): %s", mTitle.c_str(), glGetString(GL_VERSION));

    float delta = 0;

    while (!glfwWindowShouldClose(mWindow))
    {
        //glfwWaitEvents();

		float time = glfwGetTime();

        update(delta);
        draw();

        glfwSwapBuffers(mWindow);

        glfwPollEvents();

        delta = glfwGetTime() - time;
    }
}
