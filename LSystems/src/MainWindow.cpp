#include "MainWindow.h"

MainWindow::MainWindow(uint16 width, uint16 height, const std::string& title) : Window(width, height, title)
{

}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point

	mCamera = new PerspectiveCamera(70, (float)mWidth / (float)mHeight);
	mCamera->position = { 0, 0.3, 1 };
	mCamera->rotation.y = 180;

	Shader* shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");

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
	
	//LSystem mesh
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;
	mLSystem.generate(3, vertices, elements);

	mLSystemMesh = new Mesh(shader, vertices, elements);
	mLSystemMesh->position = { 0.1, 0, 0.1 };

	mLSystemMesh->setAttribute3f("aPosition", 6, 0);
	mLSystemMesh->setAttribute3f("aColor", 6, 3);
	mLSystemMesh->scale = glm::vec3(0.1, 0.1, 0.1);

	
	//Floor mesh
	mFloorMesh = new Mesh(shader, new std::vector<float>{
			-0.5, 0,  0.5, 0, 0.2, 0,
			 0.5, 0,  0.5, 0, 0.2, 0,
			-0.5, 0, -0.5, 0, 0.2, 0,
			 0.5, 0, -0.5, 0, 0.2, 0
		}, new std::vector<uint32>{
			2, 1, 0,
			2, 3, 1
	});

	mFloorMesh->setAttribute3f("aPosition", 6, 0);
	mFloorMesh->setAttribute3f("aColor", 6, 3);
	mFloorMesh->position.y = -0.001;

	//Coordinates mesh
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

	mCoordinatesMesh->setAttribute3f("aPosition", 6, 0);
	mCoordinatesMesh->setAttribute3f("aColor", 6, 3);

	//GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void MainWindow::update(float delta)
{
	if (mForward) mCamera->position += mCamera->front() * delta;
	if (mBackward) mCamera->position += mCamera->back() * delta;
	if (mLeft) mCamera->position += mCamera->left() * delta;
	if (mRight) mCamera->position += mCamera->right() * delta;
	if (mUp) mCamera->position += mCamera->up() * delta;
	if (mDown) mCamera->position += mCamera->down() * delta;
	if (mRollLeft) mCamera->rotation.z -= 100 * delta;
	if (mRollRight) mCamera->rotation.z += 100 * delta;

	if (!showmouse)
	{
		mCamera->rotation.x += (mouse.y - mouseLast.y) * 10 * delta;
		mCamera->rotation.y -= (mouse.x - mouseLast.x) * 10 * delta;
	}
	mCamera->aspect = (float)mWidth / (float)mHeight;

	mouseLast = mouse;
}

void MainWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mLSystemMesh->draw(mCamera);
	mFloorMesh->draw(mCamera);
	mCoordinatesMesh->draw(mCamera, Mesh::LINES);
}

void MainWindow::keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods)
{
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
	
}

void MainWindow::mousePositionCallback(double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;
}

void MainWindow::mouseEnteredCallback(int32 entered)
{
	
}
