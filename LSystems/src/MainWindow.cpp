#include "MainWindow.h"

MainWindow::MainWindow(uint16 width, uint16 height, const std::string& title) : Window(width, height, title)
{

}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point

	/*mLSystem = LSystem(
		"|F",
		{
			{'F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]]"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'x', {LSystem::ROTATEX, 23}},
			{'y', {LSystem::ROTATEY, 23}},
			{'z', {LSystem::ROTATEZ, 23}},
			{'X', {LSystem::ROTATEX, -23}},
			{'Y', {LSystem::ROTATEY, -23}},
			{'Z', {LSystem::ROTATEZ, -23}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}},
			{'|', {LSystem::PITCH, -90}}
		}
	);*/

	/*mLSystem = LSystem( //stairs
		"A",
		{
			{'A', "G+F+G+F+^HvG[vH]+F[vH]+G[vH]+[F]+RA"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'f', {LSystem::MOVE, 1}},
			{'G', {LSystem::DRAW, 4}},
			{'H', {LSystem::DRAW, 0.5}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}},
			{'v', {LSystem::PITCH, 90}},
			{'^', {LSystem::PITCH, -90}},
			{'+', {LSystem::YAW, 90}},
			{'-', {LSystem::YAW, -90}},
			{'R', {LSystem::YAW, 1}},
		}
	);*/

	mLSystem = LSystem( //tree
		"|F",
		{
			{'F', "F<+[vvFvF^F]>-[^FvvF]"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}},
			{'v', {LSystem::PITCH, 20}},
			{'^', {LSystem::PITCH, -20}},
			{'+', {LSystem::YAW, 40}},
			{'-', {LSystem::YAW, -40}},
			{'<', {LSystem::ROLL, 40}},
			{'>', {LSystem::ROLL, -40}},

			{'|', {LSystem::PITCH, -90}}
		}
	);

	/*mLSystem = LSystem(
		"^FFFFFFFFFFFFFFFFF",
		{
			
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'f', {LSystem::MOVE, 1}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}},
			{'v', {LSystem::PITCH, 90}},
			{'^', {LSystem::PITCH, -90}},
			{'+', {LSystem::YAW, 90}},
			{'-', {LSystem::YAW, -90}},
			{'<', {LSystem::ROLL, 90}},
			{'>', {LSystem::ROLL, -90}},
		}
	);*/
	 
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;
	mLSystem.generate(4, vertices, elements);

	mPCamera = new PerspectiveCamera(70, (float)mWidth / (float)mHeight);
	mPCamera->position = { 0, 0.3, 1 };
	mPCamera->rotation.y = 180;
	Shader* shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
	mMesh = new Mesh(shader, vertices, elements);
	mMesh->position = { 0.1, 0, 0.1 };
	mFloorMesh = new Mesh(shader, new std::vector<float>{
			-0.5, 0,  0.5, 0, 0.2, 0,
			 0.5, 0,  0.5, 0, 0.2, 0,
			-0.5, 0, -0.5, 0, 0.2, 0,
			 0.5, 0, -0.5, 0, 0.2, 0
		}, new std::vector<uint32>{
			2, 1, 0,
			2, 3, 1
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

	std::vector<float>* cylvertices = new std::vector<float>;
	std::vector<uint32>* cylelements = new std::vector<uint32>;
	std::vector<glm::vec3> cylpoints;
	std::vector<uint32> cylindices;
	Mesh::generateCylinder(0.5, 0.5, 1, cylpoints, cylindices, 10);
	for (glm::vec3& p : cylpoints)
	{
		cylvertices->push_back(p.x);
		cylvertices->push_back(p.y);
		cylvertices->push_back(p.z);
		cylvertices->push_back(1);
		cylvertices->push_back(0);
		cylvertices->push_back(0);
	}
	for (uint32 e : cylindices)
	{
		cylelements->push_back(e);
	}
	mCylinderMesh = new Mesh(shader, cylvertices, cylelements);

	mMesh->setAttribute3f("aPosition", 6, 0);
	mMesh->setAttribute3f("aColor", 6, 3);
	mMesh->scale = glm::vec3(0.1, 0.1, 0.1);

	mFloorMesh->setAttribute3f("aPosition", 6, 0);
	mFloorMesh->setAttribute3f("aColor", 6, 3);
	mFloorMesh->position.y = -0.001;

	mCoordinatesMesh->setAttribute3f("aPosition", 6, 0);
	mCoordinatesMesh->setAttribute3f("aColor", 6, 3);

	mCylinderMesh->setAttribute3f("aPosition", 6, 0);
	mCylinderMesh->setAttribute3f("aColor", 6, 3);
	mCylinderMesh->position.x = 1;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void MainWindow::update(float delta)
{
	if (mForward) mPCamera->position += mPCamera->front() * delta;
	if (mBackward) mPCamera->position += mPCamera->back() * delta;
	if (mLeft) mPCamera->position += mPCamera->left() * delta;
	if (mRight) mPCamera->position += mPCamera->right() * delta;
	if (mUp) mPCamera->position += mPCamera->up() * delta;
	if (mDown) mPCamera->position += mPCamera->down() * delta;
	if (mRollLeft) mPCamera->rotation.z -= 100 * delta;
	if (mRollRight) mPCamera->rotation.z += 100 * delta;

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

	mMesh->draw(mPCamera);
	mFloorMesh->draw(mPCamera);
	mCoordinatesMesh->draw(mPCamera, Mesh::LINES);
	//mCylinderMesh->draw(mPCamera);
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
