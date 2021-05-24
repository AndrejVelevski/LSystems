#include "MainWindow.h"

#include "Utils/Utils.h"

MainWindow::MainWindow(uint16 width, uint16 height, const std::string& title) : Window(width, height, title)
{

}

MainWindow::~MainWindow()
{
	delete mLSystemMesh;
	delete mFloorMesh;
	delete mCoordinatesMesh;
	delete mCamera;
	delete mGUICamera;

	delete mFont;
	delete mButtonGenerate;
	delete mLabelAxiom;
	delete mTextEditAxiom;
}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point

	mCamera = new PerspectiveCamera(70, (float)width() / (float)height());
	mCamera->position = { 0, 0.3, 1 };
	mCamera->rotation.y = 180;

	mGUICamera = new OrthographicCamera(0, 0, width(), height());

	mFont = new Font("res/fonts/arial.ttf", 16);

	//GUI

	int wWidth = 100;
	int wHeight = 30;

	int spacing = 50;
	int margin = 20;
	int idx = 0;

	mLabelAxiom = new Label(wWidth, wHeight, margin, margin + spacing * idx, "Axiom:", mFont, Label::HAlign::RIGHT);
	mLabelAxiom->setColor({ 1, 1, 1 });
	mTextEditAxiom = new TextEdit(6 * wWidth, wHeight, 2 * margin + wWidth, margin + spacing * idx, mFont);
	mLabelInstructions = new Label(wWidth, wHeight, 7*wWidth + 3*margin, margin + spacing * idx, "Instructions:", mFont, Label::HAlign::LEFT);
	mLabelInstructions->setColor({ 1, 1, 1 });
	mButtonGenerate = new Button(wWidth, wHeight, 8 * wWidth + 4 * margin, margin + spacing * idx, "Generate", mFont);
	mButtonGenerate->setColor({ 0.9, 0.9, 0.9 });

	++idx;

	mLabelRules = new Label(wWidth, wHeight, margin, margin + spacing * idx, "Rules:", mFont, Label::HAlign::RIGHT);
	mLabelRules->setColor({ 1, 1, 1 });

	++idx;

	

	//END GUI

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
			{'F', {LSystem::DRAW, 5}},
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

	//LSystem mesh
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;
	mLSystem.generate(4, vertices, elements);

	mLSystemMesh = new Mesh(new Shader("res/shaders/default.vert", "res/shaders/default.frag"), vertices, elements);
	mLSystemMesh->position = { 0.1, 0, 0.1 };

	mLSystemMesh->setAttribute3f("aPosition", 6, 0);
	mLSystemMesh->setAttribute3f("aColor", 6, 3);
	mLSystemMesh->scale = glm::vec3(0.1, 0.1, 0.1);

	
	//Floor mesh
	mFloorMesh = new Mesh(new Shader("res/shaders/default.vert", "res/shaders/default.frag"), new std::vector<float>{
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
	mCoordinatesMesh = new Mesh(new Shader("res/shaders/default.vert", "res/shaders/default.frag"), new std::vector<float>{
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	mCamera->aspect = (float)width() / (float)height();
	mGUICamera->right = width();
	mGUICamera->bottom = height();

	mouseLast = mouse;
	time += delta;
}

void MainWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mLSystemMesh->draw(mCamera);
	mFloorMesh->draw(mCamera);
	mCoordinatesMesh->draw(mCamera, Mesh::LINES);

	if (showmouse)
	{
		bool drawCursor = (int) fmod(time * 2, 2);

		glDisable(GL_DEPTH_TEST);
		mLabelAxiom->draw(mGUICamera);
		mLabelRules->draw(mGUICamera);
		mLabelInstructions->draw(mGUICamera);
		mTextEditAxiom->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditAxiom);
		mButtonGenerate->draw(mGUICamera);

		int wWidth = 100;
		int wHeight = 30;

		int spacing = 50;
		int margin = 20;

		if (mTextEditRules.size() == 0 || mTextEditRules[mTextEditRules.size() - 1]->text() != "")
		{
			TextEdit* textEdit = new TextEdit(6 * wWidth, wHeight, 2 * margin + wWidth, margin + spacing * (mTextEditRules.size() + 1), mFont);
			mTextEditRules.push_back(textEdit);
		}

		if (mTextEditInstructions.size() == 0 || mTextEditInstructions[mTextEditInstructions.size() - 1]->text() != "")
		{
			TextEdit* textEdit = new TextEdit(wWidth, wHeight, 7 * wWidth + 3 * margin, margin + spacing * (mTextEditInstructions.size() + 1), mFont);
			mTextEditInstructions.push_back(textEdit);
		}

		if (mTextEditRules.size() >= 2)
		{
			TextEdit* prelast = mTextEditRules[mTextEditRules.size() - 2];
			TextEdit* last = mTextEditRules[mTextEditRules.size() - 1];

			if (prelast->text() == "" && last->text() == "")
			{
				mTextEditRules.erase(mTextEditRules.end()-1);
				if (last == mFocusedTextEdit)
					mFocusedTextEdit = nullptr;
				delete last;
			}

			for (int i = mTextEditRules.size() - 2; i >= 0; --i)
			{
				if (mTextEditRules[i]->text() == "")
				{
					TextEdit* rule = mTextEditRules[i];
					mTextEditRules.erase(mTextEditRules.begin() + i);
					if (rule == mFocusedTextEdit)
						mFocusedTextEdit = nullptr;
					delete rule;

					for (int j = i; j < mTextEditRules.size(); ++j)
					{
						glm::vec2 pos = mTextEditRules[j]->getPosition();
						mTextEditRules[j]->setPosition(pos.x, pos.y - spacing);
					}
				}
			}
		}

		if (mTextEditInstructions.size() >= 2)
		{
			TextEdit* prelast = mTextEditInstructions[mTextEditInstructions.size() - 2];
			TextEdit* last = mTextEditInstructions[mTextEditInstructions.size() - 1];

			if (prelast->text() == "" && last->text() == "")
			{
				mTextEditInstructions.erase(mTextEditInstructions.end() - 1);
				if (last == mFocusedTextEdit)
					mFocusedTextEdit = nullptr;
				delete last;
			}

			for (int i = mTextEditInstructions.size() - 2; i >= 0; --i)
			{
				if (mTextEditInstructions[i]->text() == "")
				{
					TextEdit* rule = mTextEditInstructions[i];
					mTextEditInstructions.erase(mTextEditInstructions.begin() + i);
					if (rule == mFocusedTextEdit)
						mFocusedTextEdit = nullptr;
					delete rule;

					for (int j = i; j < mTextEditInstructions.size(); ++j)
					{
						glm::vec2 pos = mTextEditInstructions[j]->getPosition();
						mTextEditInstructions[j]->setPosition(pos.x, pos.y - spacing);
					}
				}
			}
		}

		for (TextEdit* rule : mTextEditInstructions)
		{
			rule->draw(mGUICamera, drawCursor && mFocusedTextEdit == rule);
		}

		for (TextEdit* rule : mTextEditRules)
		{
			rule->draw(mGUICamera, drawCursor && mFocusedTextEdit == rule);
		}

		glEnable(GL_DEPTH_TEST);
	}
}

void MainWindow::characterCallback(uint32 chr)
{
	if (showmouse)
	{
		if (mFocusedTextEdit != nullptr)
		{
			mFocusedTextEdit->addChar((int8)chr);
		}
	}
}

void MainWindow::keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (!showmouse)
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
	}  
	else
	{
		if (mFocusedTextEdit != nullptr)
		{
			if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->removeChar();
			}
			else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->setCursorPosition(mFocusedTextEdit->getCursorPosition() - 1);
			}
			else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->setCursorPosition(mFocusedTextEdit->getCursorPosition() + 1);
			}
			else if (key == GLFW_KEY_HOME && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->setCursorPosition(0);
			}
			else if (key == GLFW_KEY_END && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->setCursorPosition(mFocusedTextEdit->text().size());
			}
			else if (key == GLFW_KEY_DELETE && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->setText("");
			}
		}
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		showmouse = !showmouse;
		setCursorEnabled(showmouse);
		setCursorPosition(width() / 2, height() / 2);
		mouse.x = width() / 2;
		mouse.y = width() / 2;
		mouseLast = mouse;
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) close();
}

void MainWindow::mouseButtonsCallback(int32 button, int32 action, int32 mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		if (mButtonGenerate->contains(mouse.x, mouse.y))
		{
			Log::info("%s", Utils::openFileDialog().c_str());
			return;
		}

		if (mTextEditAxiom->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditAxiom;
			mTextEditAxiom->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		for (TextEdit* rule : mTextEditRules)
		{
			if (rule->contains(mouse.x, mouse.y))
			{
				mFocusedTextEdit = rule;
				rule->setCursorPosition(mouse.x, mouse.y);
				return;
			}
		}

		for (TextEdit* rule : mTextEditInstructions)
		{
			if (rule->contains(mouse.x, mouse.y))
			{
				mFocusedTextEdit = rule;
				rule->setCursorPosition(mouse.x, mouse.y);
				return;
			}
		}

		mFocusedTextEdit = nullptr;
	}
}

void MainWindow::mousePositionCallback(double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;

	if (mButtonGenerate->contains(xpos, ypos))
	{
		mButtonGenerate->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonGenerate->setColor({ 0.9, 0.9, 0.9 });
	}

	if (mTextEditAxiom->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}

	for (TextEdit* rule : mTextEditRules)
	{
		if (rule->contains(xpos, ypos))
		{
			setCursor(Window::IBEAM);
			return;
		}
	}

	for (TextEdit* rule : mTextEditInstructions)
	{
		if (rule->contains(xpos, ypos))
		{
			setCursor(Window::IBEAM);
			return;
		}
	}

	setCursor(Window::ARROW);
}

void MainWindow::mouseEnteredCallback(int32 entered)
{
	
}
