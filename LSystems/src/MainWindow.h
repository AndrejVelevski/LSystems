#pragma once

#include "Utils/Window.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

#include "LSystem.h"
#include "Label.h"
#include "Button.h"
#include "TextEdit.h"
#include "TrackPad.h"

class MainWindow : public Window
{
public:
	MainWindow(uint16 width, uint16 height, const std::string& title);
	~MainWindow();

private:
	void setup() override;
	void update(float delta) override;
	void draw() override;

	void generate();
	void generateFromFile(const std::string& filepath);

	void characterCallback(uint32 chr) override;
	void keyboardCallback(int32 key, int32 scancode, int32 action, int32 mods) override;
	void mouseButtonsCallback(int32 button, int32 action, int32 mods) override;
	void mousePositionCallback(double xpos, double ypos) override;
	void mouseEnteredCallback(int32 entered) override;

	Mesh* mLSystemMesh;
	Mesh* mFloorMesh;
	Mesh* mCoordinatesMesh;
	PerspectiveCamera* mCamera;
	OrthographicCamera* mGUICamera;

	Font* mFont;
	Button* mButtonOpen;
	Button* mButtonGenerate;
	Button* mButtonAutoGenerate;
	Button* mButtonCoordinateSystem;
	Label* mLabelAxiom;
	Label* mLabelRules;
	Label* mLabelInstructions;
	TextEdit* mTextEditAxiom;
	std::vector<TextEdit*> mTextEditRules;
	std::vector<TextEdit*> mTextEditInstructions;

	Label* mLabelGenerations;
	TextEdit* mTextEditGenerations;
	Label* mLabelLineThickness;
	TextEdit* mTextEditLineThickness;
	Label* mLabelLineThicknessModifier;
	TextEdit* mTextEditLineThicknessModifier;
	Label* mLabelLineLengthModifier;
	TextEdit* mTextEditLineLengthModifier;

	Label* mLabelPruneChance;
	TextEdit* mTextEditPruneChance;
	Label* mLabelMutationChance;
	TextEdit* mTextEditMutationChance;
	Label* mLabelMutationFactor;
	TextEdit* mTextEditMutationFactor;

	Label* mLabelInfo;
	Button* mButtonLightingEnabled;
	Button* mButtonDrawMode;
	TrackPad* mTrackPad;

	TextEdit* mFocusedTextEdit;

	bool mForward = false;
	bool mBackward = false;
	bool mLeft = false;
	bool mRight = false;
	bool mUp = false;
	bool mDown = false;
	bool mRollLeft = false;
	bool mRollRight = false;

	glm::vec2 mouse;
	glm::vec2 mouseLast;
	bool showmouse = true;

	bool trackPadHeld = false;

	double time = 0;
};

