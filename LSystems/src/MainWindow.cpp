#include "MainWindow.h"



MainWindow::MainWindow(uint16 width, uint16 height, const std::string& title) :
	Window(width, height, title)
{

}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point

	mLSystem = LSystem( //Islands and lakes
		"-&F+FFF",
		{
			/*{'A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//"},
			{'B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//"},
			{'C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//"},
			{'D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//"}*/
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::YAW, 90}},
			{'-', {LSystem::YAW, -90}},
			{'&', {LSystem::PITCH, 45}},
			{'^', {LSystem::PITCH, -90}},
			{'<', {LSystem::ROLL, 45}},
			{'>', {LSystem::ROLL, -90}},
			{'|', {LSystem::YAW, 180}}
		}
	);
	 
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;
	mLSystem.generate(0, vertices, elements);

	mPCamera = new PerspectiveCamera(70, (float)mWidth / (float)mHeight);
	mPCamera->position = { 0, 0.3, -1 };
	Shader* shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
	mMesh = new Mesh(shader, vertices, elements);
	mFloorMesh = new Mesh(shader, new std::vector<float>{
			-0.5, 0,  0.5, 0, 0.2, 0,
			 0.5, 0,  0.5, 0, 0.2, 0,
			-0.5, 0, -0.5, 0, 0.2, 0,
			 0.5, 0, -0.5, 0, 0.2, 0
		}, new std::vector<uint32>{
			0, 1, 2,
			1, 3, 2
	});
	mCoordinatesMesh = new Mesh(shader, new std::vector<float>{
			0, 0, 0, 1, 0, 0,
			1, 0, 0, 1, 0, 0,
			0, 0, 0, 0, 1, 0,
			0, 1, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 1,
			0, 0, 1, 0, 0, 1,
		}, new std::vector<uint32>{
			0, 1,
			2, 3,
			4, 5
	});

	mMesh->setAttribute3f("aPosition", 6, 0);
	mMesh->setAttribute3f("aColor", 6, 3);
	mMesh->scale = glm::vec3(0.1, 0.1, 0.1);

	mFloorMesh->setAttribute3f("aPosition", 6, 0);
	mFloorMesh->setAttribute3f("aColor", 6, 3);
	mFloorMesh->position.y = -0.001;

	mCoordinatesMesh->setAttribute3f("aPosition", 6, 0);
	mCoordinatesMesh->setAttribute3f("aColor", 6, 3);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void MainWindow::update(float delta)
{
	if (mForward) mPCamera->position += mPCamera->front() * delta;
	if (mBackward) mPCamera->position -= mPCamera->front() * delta;
	if (mLeft) mPCamera->position -= mPCamera->right() * delta;
	if (mRight) mPCamera->position += mPCamera->right() * delta;
	if (mUp) mPCamera->position += mPCamera->up() * delta;
	if (mDown) mPCamera->position -= mPCamera->up() * delta;
	if (mRollLeft) mPCamera->rotation.z += 100 * delta;
	if (mRollRight) mPCamera->rotation.z -= 100 * delta;

	if (!showmouse)
	{
		mPCamera->rotation.x += (mouse.y - mouseLast.y) * 10 * delta;
		mPCamera->rotation.y -= (mouse.x - mouseLast.x) * 10 * delta;
	}
	mPCamera->aspect = (float)mWidth / (float)mHeight;

	//Log::info("%d %d", mWidth, mHeight);
	//mMesh->position = { 1,0,0 };
	//mMesh->position = { mWidth / 2, mHeight / 2, 0 };
	//mMesh->rotation.y += 100 * delta;

	//mFloorMesh->rotation.y = 90;
	//mFloorMesh->rotation.x += 100*delta;

	//mCoordinatesMesh->rotation.y = 90;
	//mCoordinatesMesh->rotation.x += 100 * delta;

	mouseLast = mouse;
}

void MainWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mMesh->draw(mPCamera, Mesh::LINES);
	mFloorMesh->draw(mPCamera);
	mCoordinatesMesh->draw(mPCamera, Mesh::LINES);
}

void MainWindow::keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods)
{
	//Log::info("(%s): %d %d %d %d", mTitle, key, scancode, action, mods);

	     if (key == GLFW_KEY_W && action == GLFW_PRESS) mForward = true;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) mBackward = true;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) mLeft = true;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) mRight = true;
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) mRollLeft = true;
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) mRollRight = true;
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) mUp = true;
	else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) mDown = true;

	else if (key == GLFW_KEY_W && action == GLFW_RELEASE) mForward = false;
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE) mBackward = false;
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE) mLeft = false;
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE) mRight = false;
	else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) mRollLeft = false;
	else if (key == GLFW_KEY_E && action == GLFW_RELEASE) mRollRight = false;
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) mUp = false;
	else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) mDown = false;

	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		showmouse = !showmouse;
		glfwSetInputMode(mWindow, GLFW_CURSOR, showmouse?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(mWindow, mWidth/2, mHeight/2);
		mouse.x = mWidth / 2;
		mouse.y = mWidth / 2;
		mouseLast = mouse;
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) close();
}

void MainWindow::mouseButtonsCallback(int32 button, int32 action, int32 mods)
{
	//Log::info("(%s): %d %d %d", mTitle, button, action, mods);
}

void MainWindow::mousePositionCallback(double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;
}

void MainWindow::mouseEnteredCallback(int32 entered)
{
	//Log::info("(%s): %d", mTitle, entered);
}
