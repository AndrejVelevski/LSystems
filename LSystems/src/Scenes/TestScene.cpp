#include "TestScene.h"

#include <glad\glad.h>

#include "../Utils/Log.h";
#include "../Utils/Types.h"

#include <iostream>

TestScene::TestScene()
{
	std::vector<float> vertices {
		-0.5, -0.5, 0.0,
		 0.5, -0.5, 0.0,
		 0.0,  0.5, 0.0,
	};

	std::vector<uint32> elements {
		0, 1, 2
	};

	mCamera = new PerspectiveCamera;
	mCamera->position.z = 3;
	mMesh = new Mesh(vertices, elements);
	mShader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
	mMesh->bind();
	mShader->setAttribute3f("aPosition", 3 * sizeof(float), 0);
	mMesh->unbind();
}

void TestScene::update(float delta) const
{
	mMesh->rotation.y += 100 * delta;
	mMesh->update(delta);
}

void TestScene::draw() const
{
	glClearColor(0.3, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	mMesh->draw(*mShader, *mCamera);
}
