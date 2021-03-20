#pragma once

#include "../Renderer/IScene.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Shader.h"
#include "../Renderer/PerspectiveCamera.h"

class TestScene : public IScene
{
public:
	TestScene();

	virtual void update(float delta) const override;
	virtual void draw() const override;
	
private:
	Mesh* mMesh;
	Shader* mShader;
	PerspectiveCamera* mCamera;
};

