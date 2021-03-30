#include "MainWindow.h"

MainWindow::MainWindow(uint16 width, uint16 height, const std::string& title) :
	Window(width, height, title)
{

}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point
	std::vector<float>* vertices1 = new std::vector<float> {
		-0.5,  0.5, 0.0, 1.0, 0.0, 0.0,
		 0.5,  0.5, 0.0, 0.0, 1.0, 0.0,
		-0.5, -0.5, 0.0, 0.0, 0.0, 1.0,
		 0.5, -0.5, 0.0, 1.0, 1.0, 1.0,
	};

	std::vector<float>* elements1 = new std::vector<float>{
		-50, -50, 0,
		 50, -50, 0,
		-50,  50, 0,
		 50,  50, 0,
	};

	std::vector<uint32>* elements = new std::vector<uint32>{
		0, 1, 2,
		1, 3, 2
	};

	mPCamera = new PerspectiveCamera(70, (float)mWidth / (float)mHeight);
	mOCamera = new OrthographicCamera(0, mWidth, 0, mHeight);
	mPCamera->position.z = 3;
	mOCamera->position.z = 1;
	Shader* shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
	mMesh = new Mesh(shader, vertices1, elements);

	mMesh->setAttribute3f("aPosition", 6, 0);
	mMesh->setAttribute3f("aColor", 6, 3);
}

void MainWindow::update(float delta)
{
	mPCamera->aspect = (float)mWidth / (float)mHeight;
	mOCamera->left = 0;
	mOCamera->right = mWidth;
	mOCamera->top = 0;
	mOCamera->bottom = mHeight;
	//Log::info("%d %d", mWidth, mHeight);
	//mMesh->position = { 1,0,0 };
	//mMesh->position = { mWidth / 2, mHeight / 2, 0 };
	mMesh->rotation.y += 100 * delta;
}

void MainWindow::draw()
{
	glClearColor(0.3, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	mMesh->draw(mPCamera);
}

void MainWindow::keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods) const
{
	//Log::info("(%s): %d %d %d %d", mTitle, key, scancode, action, mods);
}

void MainWindow::mouseButtonsCallback(int32 button, int32 action, int32 mods) const
{
	//Log::info("(%s): %d %d %d", mTitle, button, action, mods);
}

void MainWindow::mousePositionCallback(double xpos, double ypos) const
{
	//Log::info("(%s): %f %f", mTitle, xpos, ypos);
}

void MainWindow::mouseEnteredCallback(int32 entered) const
{
	//Log::info("(%s): %d", mTitle, entered);
}
