#include "MainWindow.h"

#include "Utils/Utils.h"

#include <unordered_map>
#include <fstream>

int wWidth = 160;
int wHeight = 20;

int margin = 10;
int spacing = wHeight + margin;

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
	delete mTrackPad;

	for (TextEdit* textEdit : mTextEditRules)
		delete textEdit;

	for (TextEdit* textEdit : mTextEditInstructions)
		delete textEdit;
}

void MainWindow::setup()
{
	//TODO: this should be in the constructor, but glad isn't initialized at that point

	mCamera = new PerspectiveCamera(70, (float)width() / (float)height());
	mCamera->position = { 0, 0.3, 1 };
	mCamera->rotation.y = 180;

	mGUICamera = new OrthographicCamera(0, 0, width(), height());

	mFont = new Font("res/fonts/arial.ttf", 12);

	//GUI
	int idx = 0;

	mLabelAxiom = new Label(wWidth/4, wHeight, margin, margin + spacing * idx, "Axiom:", mFont, Label::HAlign::RIGHT);
	mTextEditAxiom = new TextEdit(3 * wWidth, wHeight, 2 * margin + wWidth - wWidth * 0.75, margin + spacing * idx, mFont);
	mLabelInstructions = new Label(wWidth, wHeight, 4*wWidth + 3*margin - wWidth * 0.75, margin + spacing * idx, "Instructions:", mFont, Label::HAlign::LEFT);
	mButtonAutoGenerate = new Button(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Auto-generate: on", mFont);
	mButtonGenerate = new Button(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, "Generate", mFont);
	mButtonGenerate->setColor({ 0.9, 0.9, 0.9 });

	++idx;

	mLabelRules = new Label(wWidth/4, wHeight, margin, margin + spacing * idx, "Rules:", mFont, Label::HAlign::RIGHT);

	mButtonOpen = new Button(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Open", mFont);
	mButtonCoordinateSystem = new Button(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, "Show coordinate system", mFont);

	++idx;

	mLabelGenerations = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Generations:", mFont, Label::HAlign::RIGHT);
	mTextEditGenerations = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditGenerations->setText("3");

	++idx;

	mLabelLineThickness = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Line thickness:", mFont, Label::HAlign::RIGHT);
	mTextEditLineThickness = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditLineThickness->setText("0.1");

	++idx;

	mLabelLineThicknessModifier = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Line thickness modifier:", mFont, Label::HAlign::RIGHT);
	mTextEditLineThicknessModifier = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditLineThicknessModifier->setText("1");

	++idx;

	mLabelLineLengthModifier = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Line length modifier:", mFont, Label::HAlign::RIGHT);
	mTextEditLineLengthModifier = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditLineLengthModifier->setText("1");

	++idx;

	mLabelPruneChance = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Prune chance:", mFont, Label::HAlign::RIGHT);
	mTextEditPruneChance = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditPruneChance->setText("0");

	++idx;

	mLabelMutationChance = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Mutation chance:", mFont, Label::HAlign::RIGHT);
	mTextEditMutationChance = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditMutationChance->setText("0");

	++idx;

	mLabelMutationFactor = new Label(wWidth, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, "Mutation factor:", mFont, Label::HAlign::RIGHT);
	mTextEditMutationFactor = new TextEdit(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, mFont);
	mTextEditMutationFactor->setText("0");

	++idx;

	mTrackPad = new TrackPad(wWidth, wWidth, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx);

	mButtonLightingEnabled = new Button(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, "Lighting: on", mFont);

	++idx;

	mButtonDrawMode = new Button(wWidth, wHeight, 6 * wWidth + 5 * margin - wWidth * 0.75, margin + spacing * idx, "Draw: Mesh", mFont);

	idx += 5;

	std::string info =
		"Controls:\n\n"
		"W - Forwards\n"
		"S - Backwards\n"
		"A - Left\n"
		"D - Right\n"
		"Q - Roll left\n"
		"E - Roll right\n"
		"Space - Up\n"
		"Ctrl - Down\n\n"
		"Enter - Toggle between camera mode and UI\n"
		"R - Reset camera to default position and orientation\n"
		"T - Generate L-System";
	mLabelInfo = new Label(2*wWidth+margin, wHeight, 5 * wWidth + 4 * margin - wWidth * 0.75, margin + spacing * idx, info, mFont, Label::HAlign::LEFT, Label::VAlign::TOP);

	for (int i=0;i<11;++i)
		mTextEditInstructions.push_back(new TextEdit(wWidth, wHeight, 4 * wWidth + 3 * margin - wWidth * 0.75, margin + spacing * (i + 1), mFont));
	mTextEditInstructions[0]->setText("F: DRAW, 1");
	mTextEditInstructions[1]->setText("f: MOVE, 1");
	mTextEditInstructions[2]->setText("[: PUSH, 0");
	mTextEditInstructions[3]->setText("]: POP, 0");
	mTextEditInstructions[4]->setText("v: PITCH, 30");
	mTextEditInstructions[5]->setText("^: PITCH, -30");
	mTextEditInstructions[6]->setText("+: YAW, 30");
	mTextEditInstructions[7]->setText("-: YAW, -30");
	mTextEditInstructions[8]->setText("<: ROLL, 30");
	mTextEditInstructions[9]->setText(">: ROLL, -30");
	mTextEditInstructions[10]->setText("|: PITCH, -90");

	//END GUI

	mLSystemMesh = new Mesh(new Shader("res/shaders/phong.vert", "res/shaders/phong.frag"), nullptr, nullptr);

	//Floor mesh
	mFloorMesh = new Mesh(new Shader("res/shaders/phong.vert", "res/shaders/phong.frag"), new std::vector<float>{
			-0.5, 0,  0.5, 0, 1, 0, 0, 0.2, 0,
			 0.5, 0,  0.5, 0, 1, 0, 0, 0.2, 0,
			-0.5, 0, -0.5, 0, 1, 0, 0, 0.2, 0,
			 0.5, 0, -0.5, 0, 1, 0, 0, 0.2, 0
		}, new std::vector<uint32>{
			2, 0, 1,
			2, 1, 3
	});

	mFloorMesh->setAttribute3f("aPosition", 9, 0);
	mFloorMesh->setAttribute3f("aNormal", 9, 3);
	mFloorMesh->setAttribute3f("aColor", 9, 6);
	mFloorMesh->setUniform3f("uColor", { 1, 1 ,1 });
	mFloorMesh->position.y = -0.001;

	//Coordinates mesh
	mCoordinatesMesh = new Mesh(new Shader("res/shaders/phong.vert", "res/shaders/phong.frag"), new std::vector<float>{
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
	glClearColor(0.5, 0.5, 0.5, 1.0);
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
		setCursorPosition(mouse.x, mouse.y);
		mCamera->rotation.x += (mouse.y - mouseLast.y) * 10 * delta;
		mCamera->rotation.y -= (mouse.x - mouseLast.x) * 10 * delta;
	}

	float aspect = (float)width() / (float)height();
	if (isnan(aspect))
		aspect = 1;

	mCamera->aspect = aspect;
	mGUICamera->right = width();
	mGUICamera->bottom = height();

	mouseLast = mouse;
	time += delta;
}

void MainWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bool lighting = mButtonLightingEnabled->getText() == "Lighting: on";

	glDisable(GL_CULL_FACE);
	mFloorMesh->setUniform1i("uLightingEnabled", lighting);
	mFloorMesh->setUniform3f("uLight.direction", { mTrackPad->getTrackerPosition().x, -1, mTrackPad->getTrackerPosition().y});
	mFloorMesh->draw(mCamera);

	if (mButtonDrawMode->getText() == "Draw: Mesh")
	{
		glEnable(GL_CULL_FACE);
		mLSystemMesh->scale = { 0.1, 0.1, 0.1 };
		mLSystemMesh->setUniform1i("uLightingEnabled", lighting);
		mLSystemMesh->setUniform3f("uLight.direction", { mTrackPad->getTrackerPosition().x, -1, mTrackPad->getTrackerPosition().y });
		mLSystemMesh->setUniform3f("uColor", { 1, 1, 1 });
		mLSystemMesh->draw(mCamera);
	}
	else if (mButtonDrawMode->getText() == "Draw: Wireframe")
	{
		mLSystemMesh->scale = { 0.1, 0.1, 0.1 };
		mLSystemMesh->setUniform1i("uLightingEnabled", 0);
		mLSystemMesh->setUniform3f("uLight.direction", { mTrackPad->getTrackerPosition().x, -1, mTrackPad->getTrackerPosition().y });
		mLSystemMesh->setUniform3f("uColor", { 0, 0, 0 });
		mLSystemMesh->draw(mCamera, Mesh::LINES);
	}
	else if (mButtonDrawMode->getText() == "Draw: Mesh + Wireframe")
	{
		mLSystemMesh->scale = { 0.1, 0.1, 0.1 };
		glEnable(GL_CULL_FACE);
		mLSystemMesh->setUniform1i("uLightingEnabled", lighting);
		mLSystemMesh->setUniform3f("uLight.direction", { mTrackPad->getTrackerPosition().x, -1, mTrackPad->getTrackerPosition().y });
		mLSystemMesh->setUniform3f("uColor", { 1, 1, 1 });
		mLSystemMesh->draw(mCamera);

		mLSystemMesh->scale = { 0.101, 0.101, 0.101 };
		mLSystemMesh->setUniform1i("uLightingEnabled", 0);
		mLSystemMesh->setUniform3f("uColor", { 0, 0, 0 });
		mLSystemMesh->draw(mCamera, Mesh::LINES);
	}
	else if (mButtonDrawMode->getText() == "Draw: Lines")
	{
		glEnable(GL_CULL_FACE);
		mLSystemMesh->scale = { 0.1, 0.1, 0.1 };
		mLSystemMesh->setUniform1i("uLightingEnabled", 0);
		mLSystemMesh->setUniform3f("uColor", { 1, 1, 1 });
		mLSystemMesh->draw(mCamera, Mesh::LINES);
	}
	
	if (mButtonCoordinateSystem->getText() == "Hide coordinate system")
		mCoordinatesMesh->draw(mCamera, Mesh::LINES);

	if (trackPadHeld)
		mTrackPad->setTrackerPosition({ mouse.x, mouse.y });

	if (showmouse)
	{
		bool drawCursor = (int) fmod(time * 2, 2);

		glDisable(GL_DEPTH_TEST);
		mLabelAxiom->draw(mGUICamera);
		mLabelRules->draw(mGUICamera);
		mLabelInstructions->draw(mGUICamera);
		mTextEditAxiom->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditAxiom);
		mButtonGenerate->draw(mGUICamera);
		mButtonAutoGenerate->draw(mGUICamera);
		mButtonOpen->draw(mGUICamera);
		mButtonCoordinateSystem->draw(mGUICamera);

		mLabelGenerations->draw(mGUICamera);
		mLabelLineThickness->draw(mGUICamera);
		mLabelLineThicknessModifier->draw(mGUICamera);
		mLabelLineLengthModifier->draw(mGUICamera);
		mLabelPruneChance->draw(mGUICamera);
		mLabelMutationChance->draw(mGUICamera);
		mLabelMutationFactor->draw(mGUICamera);

		mTextEditGenerations->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditGenerations);
		mTextEditLineThickness->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditLineThickness);
		mTextEditLineThicknessModifier->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditLineThicknessModifier);
		mTextEditLineLengthModifier->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditLineLengthModifier);
		mTextEditPruneChance->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditPruneChance);
		mTextEditMutationChance->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditMutationChance);
		mTextEditMutationFactor->draw(mGUICamera, drawCursor && mFocusedTextEdit == mTextEditMutationFactor);

		mTrackPad->draw(mGUICamera);
		mButtonLightingEnabled->draw(mGUICamera);
		mButtonDrawMode->draw(mGUICamera);
		mLabelInfo->draw(mGUICamera);

		if (mTextEditRules.size() == 0 || mTextEditRules[mTextEditRules.size() - 1]->text() != "")
		{
			TextEdit* textEdit = new TextEdit(3 * wWidth, wHeight, 2 * margin + wWidth - wWidth * 0.75, margin + spacing * (mTextEditRules.size() + 1), mFont);
			mTextEditRules.push_back(textEdit);
		}

		if (mTextEditInstructions.size() == 0 || mTextEditInstructions[mTextEditInstructions.size() - 1]->text() != "")
		{
			TextEdit* textEdit = new TextEdit(wWidth, wHeight, 4 * wWidth + 3 * margin - wWidth * 0.75, margin + spacing * (mTextEditInstructions.size() + 1), mFont);
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

void MainWindow::generate()
{
	std::string axiom = mTextEditAxiom->text();
	std::unordered_map<char, std::string> rules;
	std::unordered_map<char, std::pair<LSystem::Instruction, float>> instructions;

	for (int i=0;i<mTextEditRules.size()-1;++i)
	{
		std::string text = mTextEditRules[i]->text();
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());
		if (text.size() == 0 || !Utils::contains(text, ':'))
			return;
		std::vector<std::string> split = Utils::split(text, ":");
		if (split.size() == 0)
			return;
		rules.insert(std::pair<char, std::string>(split[0][0], split[1]));
	}

	for (int i = 0; i < mTextEditInstructions.size() - 1; ++i)
	{
		std::string text = mTextEditInstructions[i]->text();
		text.erase(std::remove(text.begin(), text.end(), ' '), text.end());
		if (text.size() == 0 || !Utils::contains(text, ':'))
			return;
		std::vector<std::string> split = Utils::split(text, ":");
		if (split.size() == 0)
			return;
		char c = split[0][0];

		if (split[1].size() == 0)
			return;

		LSystem::Instruction instruction;
		float value = 0;

		if (Utils::contains(split[1], ','))
		{
			split = Utils::split(split[1], ",");

			if (split.size() != 2)
				return;

			if (Utils::toUpper(split[0]) == "DRAW")
				instruction = LSystem::DRAW;
			else if (Utils::toUpper(split[0]) == "MOVE")
				instruction = LSystem::MOVE;
			else if (Utils::toUpper(split[0]) == "ROTATEZ" || Utils::toUpper(split[0]) == "ROLL")
				instruction = LSystem::ROTATEZ;
			else if (Utils::toUpper(split[0]) == "ROTATEY" || Utils::toUpper(split[0]) == "YAW")
				instruction = LSystem::ROTATEY;
			else if (Utils::toUpper(split[0]) == "ROTATEX" || Utils::toUpper(split[0]) == "PITCH")
				instruction = LSystem::ROTATEX;
			else if (Utils::toUpper(split[0]) == "PUSH")
				instruction = LSystem::PUSH;
			else if (Utils::toUpper(split[0]) == "POP")
				instruction = LSystem::POP;

			try
			{
				value = std::stof(split[1]);
			}
			catch (...) { }
				
		}

		instructions.insert(std::pair<char, std::pair<LSystem::Instruction, float>>(c, std::pair<LSystem::Instruction, float>(instruction, value)));
	}

	int generations = 1;
	float lineThickness = 1;
	float lineThicknessModifier = 1;
	float lineLengthModifier = 1;
	float pruneChance = 0;
	float mutationChance = 0;
	float mutationFactor = 0;

	try { generations = std::stof(mTextEditGenerations->text()); } catch (...) { }
	try { lineThickness = std::stof(mTextEditLineThickness->text()); } catch (...) { }
	try { lineThicknessModifier = std::stof(mTextEditLineThicknessModifier->text()); } catch (...) { }
	try { lineLengthModifier = std::stof(mTextEditLineLengthModifier->text()); } catch (...) { }
	try { pruneChance = std::stof(mTextEditPruneChance->text()); } catch (...) { }
	try { mutationChance = std::stof(mTextEditMutationChance->text()); } catch (...) { }
	try { mutationFactor = std::stof(mTextEditMutationFactor->text()); } catch (...) { }

	LSystem lsystem(axiom, rules, instructions);
	delete mLSystemMesh;
	bool lines = mButtonDrawMode->getText() == "Draw: Lines";

	mLSystemMesh = lsystem.generate(generations, lineThickness, lineThicknessModifier, lineLengthModifier, pruneChance, mutationChance, mutationFactor, lines);
}

void MainWindow::generateFromFile(const std::string& filepath)
{
	if (filepath == "")
		return; 

	enum class Sector {
		AXIOM,
		RULES,
		INSTRUCTIONS,
		SETTINGS
	};

	std::ifstream input(filepath);

	std::string axiom;
	std::vector<std::string> rules;
	std::vector<std::string> instructions;

	std::string generations;
	std::string lineThickness;
	std::string lineThicknessModifier;
	std::string lineLengthModifier;
	std::string pruneChance;
	std::string mutationChance;
	std::string mutationFactor;

	Sector sector = Sector::AXIOM;

	for (std::string line; getline(input, line);)
	{
		if (line.size() == 0)
			continue;

		if (line[0] == '#')
		{
			     if (Utils::toLower(line) == "#axiom") sector = Sector::AXIOM;
			else if (Utils::toLower(line) == "#rules") sector = Sector::RULES;
			else if (Utils::toLower(line) == "#instructions") sector = Sector::INSTRUCTIONS;
			else if (Utils::toLower(line) == "#settings") sector = Sector::SETTINGS;
			continue;
		}

		switch (sector)
		{
		case Sector::AXIOM: axiom = line; break;
		case Sector::RULES: rules.push_back(line); break;
		case Sector::INSTRUCTIONS: instructions.push_back(line); break;
		case Sector::SETTINGS:
		{
			std::vector<std::string> split = Utils::split(line, ":");

			     if (Utils::toLower(split[0]) == "generations") generations = split[1];
			else if (Utils::toLower(split[0]) == "linethickness") lineThickness = split[1];
			else if (Utils::toLower(split[0]) == "linethicknessmodifier") lineThicknessModifier = split[1];
			else if (Utils::toLower(split[0]) == "linelengthmodifier") lineLengthModifier = split[1];
			else if (Utils::toLower(split[0]) == "prunechance") pruneChance = split[1];
			else if (Utils::toLower(split[0]) == "mutationchance") mutationChance = split[1];
			else if (Utils::toLower(split[0]) == "mutationfactor") mutationFactor = split[1];

			break;
		}
		}
	}

	mTextEditAxiom->setText(axiom);
	mTextEditGenerations->setText(generations);
	mTextEditLineThickness->setText(lineThickness);
	mTextEditLineThicknessModifier->setText(lineThicknessModifier);
	mTextEditLineLengthModifier->setText(lineLengthModifier);
	mTextEditPruneChance->setText(pruneChance);
	mTextEditMutationChance->setText(mutationChance);
	mTextEditMutationFactor->setText(mutationFactor);

	for (TextEdit* rule : mTextEditRules)
		delete rule;
	mTextEditRules.clear();
	for (TextEdit* instruction : mTextEditRules)
		delete instruction;
	mTextEditInstructions.clear();

	for (int i = 0; i <= rules.size(); ++i)
	{
		TextEdit* textEdit = new TextEdit(3 * wWidth, wHeight, 2 * margin + wWidth - wWidth * 0.75, margin + spacing * (mTextEditRules.size() + 1), mFont);
		mTextEditRules.push_back(textEdit);
		if (i < rules.size())
			textEdit->setText(rules[i]);
	}

	for (int i = 0; i <= instructions.size(); ++i)
	{
		TextEdit* textEdit = new TextEdit(wWidth, wHeight, 4 * wWidth + 3 * margin - wWidth * 0.75, margin + spacing * (mTextEditInstructions.size() + 1), mFont);
		mTextEditInstructions.push_back(textEdit);
		if (i < instructions.size())
			textEdit->setText(instructions[i]);
	}

	generate();
}

void MainWindow::characterCallback(uint32 chr)
{
	if (showmouse)
	{
		if (mFocusedTextEdit != nullptr)
		{
			mFocusedTextEdit->addChar((int8)chr);
			if (mButtonAutoGenerate->getText() == "Auto-generate: on")
				generate();
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

		else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			mCamera->position = { 0, 0.3, 1 };
			mCamera->rotation = { 0, 180, 0 };
		}
		else if (key == GLFW_KEY_T && action == GLFW_PRESS)
		{
			generate();
		}
	}  
	else
	{
		if (mFocusedTextEdit != nullptr)
		{
			if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				mFocusedTextEdit->removeChar();

				if (mButtonAutoGenerate->getText() == "Auto-generate: on")
					generate();
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
		mouse.x = width() / 2;
		mouse.y = width() / 2;
		mouseLast = mouse;
		setCursorEnabled(showmouse);
		setCursorPosition(mouse.x, mouse.y);
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) close();
}

void MainWindow::mouseButtonsCallback(int32 button, int32 action, int32 mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		trackPadHeld = false;

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		if (mTrackPad->contains(mouse.x, mouse.y))
		{
			trackPadHeld = true;
			mTrackPad->setTrackerPosition({ mouse.x, mouse.y });
			return;
		}

		if (mButtonDrawMode->contains(mouse.x, mouse.y))
		{
			if (mButtonDrawMode->getText() == "Draw: Mesh")
				mButtonDrawMode->setText("Draw: Wireframe");
			else if (mButtonDrawMode->getText() == "Draw: Wireframe")
				mButtonDrawMode->setText("Draw: Mesh + Wireframe");
			else if (mButtonDrawMode->getText() == "Draw: Mesh + Wireframe")
			{
				mButtonDrawMode->setText("Draw: Lines");
				generate();
			}
			else if (mButtonDrawMode->getText() == "Draw: Lines")
			{
				mButtonDrawMode->setText("Draw: Mesh");
				generate();
			}
			return;
		}

		if (mButtonLightingEnabled->contains(mouse.x, mouse.y))
		{
			if (mButtonLightingEnabled->getText() == "Lighting: off")
				mButtonLightingEnabled->setText("Lighting: on");
			else
				mButtonLightingEnabled->setText("Lighting: off");
			return;
		}

		if (mButtonAutoGenerate->contains(mouse.x, mouse.y))
		{
			if (mButtonAutoGenerate->getText() == "Auto-generate: off")
			{
				mButtonAutoGenerate->setText("Auto-generate: on");
				generate();
			}
			else
				mButtonAutoGenerate->setText("Auto-generate: off");
			return;
		}

		if (mButtonGenerate->contains(mouse.x, mouse.y))
		{
			generate();
			return;
		}

		if (mButtonOpen->contains(mouse.x, mouse.y))
		{
			generateFromFile(Utils::openFileDialog());
			return;
		}

		if (mButtonCoordinateSystem->contains(mouse.x, mouse.y))
		{
			if (mButtonCoordinateSystem->getText() == "Show coordinate system")
				mButtonCoordinateSystem->setText("Hide coordinate system");
			else
				mButtonCoordinateSystem->setText("Show coordinate system");
			return;
		}

		if (mTextEditAxiom->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditAxiom;
			mTextEditAxiom->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditGenerations->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditGenerations;
			mTextEditGenerations->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditLineThickness->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditLineThickness;
			mTextEditLineThickness->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditLineThicknessModifier->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditLineThicknessModifier;
			mTextEditLineThicknessModifier->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditLineLengthModifier->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditLineLengthModifier;
			mTextEditLineLengthModifier->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditPruneChance->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditPruneChance;
			mTextEditPruneChance->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditMutationChance->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditMutationChance;
			mTextEditMutationChance->setCursorPosition(mouse.x, mouse.y);
			return;
		}

		if (mTextEditMutationFactor->contains(mouse.x, mouse.y))
		{
			mFocusedTextEdit = mTextEditMutationFactor;
			mTextEditMutationFactor->setCursorPosition(mouse.x, mouse.y);
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

	if (mTrackPad->contains(xpos, ypos))
	{
		mTrackPad->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mTrackPad->setColor({ 1, 1, 1 });
	}

	if (mButtonDrawMode->contains(xpos, ypos))
	{
		mButtonDrawMode->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonDrawMode->setColor({ 1, 1, 1 });
	}

	if (mButtonLightingEnabled->contains(xpos, ypos))
	{
		mButtonLightingEnabled->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonLightingEnabled->setColor({ 1, 1, 1 });
	}

	if (mButtonAutoGenerate->contains(xpos, ypos))
	{
		mButtonAutoGenerate->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonAutoGenerate->setColor({ 1, 1, 1 });
	}

	if (mButtonGenerate->contains(xpos, ypos))
	{
		mButtonGenerate->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonGenerate->setColor({ 1, 1, 1 });
	}

	if (mButtonOpen->contains(xpos, ypos))
	{
		mButtonOpen->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonOpen->setColor({ 1, 1, 1 });
	}

	if (mButtonCoordinateSystem->contains(xpos, ypos))
	{
		mButtonCoordinateSystem->setColor({ 0.8, 0.8, 0.8 });
		setCursor(Window::HAND);
		return;
	}
	else
	{
		mButtonCoordinateSystem->setColor({ 1, 1, 1 });
	}

	if (mTextEditAxiom->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditGenerations->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditLineThickness->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditLineThicknessModifier->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditLineLengthModifier->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditPruneChance->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditMutationChance->contains(xpos, ypos))
	{
		setCursor(Window::IBEAM);
		return;
	}
	else if (mTextEditMutationFactor->contains(xpos, ypos))
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
