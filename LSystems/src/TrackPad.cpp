#include "TrackPad.h"

#include <vector>

TrackPad::TrackPad(float width, float height, float x, float y)
{
	mWidth = width;
	mHeight = height;

	std::vector<float>* verticesPad = new std::vector<float>;
	std::vector<float>* verticesTracker = new std::vector<float>;

	int precision = 32;
	float increment = -glm::radians(360.0f / precision);
	float angle = 0;

	verticesPad->push_back(0);
	verticesPad->push_back(0);

	for (int i = 0; i <= precision; ++i)
	{
		float xx = cos(angle);
		float yy = sin(angle);

		verticesPad->push_back(xx);
		verticesPad->push_back(yy);

		verticesTracker->push_back(xx/10.0f);
		verticesTracker->push_back(yy/10.0f);

		angle += increment;
	}

	mMeshPad = new Mesh(new Shader("res/shaders/guiButton.vert", "res/shaders/guiButton.frag"), verticesPad, nullptr);
	mMeshPad->setAttribute2f("aPosition", 2, 0);
	mMeshPad->setUniform3f("uColor", { 1, 1, 1 });

	mMeshPad->scale.x = mWidth / 2;
	mMeshPad->scale.y = mHeight / 2;
	mMeshPad->position.x = x + mWidth / 2;
	mMeshPad->position.y = y + mHeight / 2;

	mMeshTracker = new Mesh(new Shader("res/shaders/guiButton.vert", "res/shaders/guiButton.frag"), verticesTracker, nullptr);
	mMeshTracker->setAttribute2f("aPosition", 2, 0);
	mMeshTracker->setUniform3f("uColor", { 0, 0, 0 });

	setTrackerPosition({ x + mWidth / 2, y + mHeight / 2 });
	float min = mWidth < mHeight ? mWidth : mHeight;
	mMeshTracker->scale.x = min / 2;
	mMeshTracker->scale.y = min / 2;
}

TrackPad::~TrackPad()
{
	delete mMeshPad;
	delete mMeshTracker;
}

glm::vec2 TrackPad::getTrackerPosition()
{
	glm::vec2 pos;

	pos.x = (mMeshTracker->position.x - mMeshPad->position.x) / mWidth;
	pos.y = -(mMeshTracker->position.y - mMeshPad->position.y) / mHeight;

	pos *= 2;

	return pos;
}

void TrackPad::setTrackerPosition(glm::vec2 position)
{
	if (contains(position.x, position.y))
	{
		mMeshTracker->position.x = position.x;
		mMeshTracker->position.y = position.y;
		return;
	}

	float direction = atan2(position.y - mMeshPad->position.y, position.x - mMeshPad->position.x);
	mMeshTracker->position.x = mMeshPad->position.x + mWidth * cos(direction) / 2;
	mMeshTracker->position.y = mMeshPad->position.y + mHeight * sin(direction) / 2;
}

bool TrackPad::contains(float x, float y)
{
	x -= mMeshPad->position.x;
	y -= mMeshPad->position.y;

	float r1 = mWidth / 2;
	float r2 = mHeight / 2;

	return x * x + y * y < r1* r2;
}

void TrackPad::setColor(glm::vec3 color)
{
	mMeshPad->setUniform3f("uColor", color);
}

void TrackPad::draw(ICamera* camera)
{
	mMeshPad->draw(camera, Mesh::TRIANGLE_FAN);
	mMeshTracker->draw(camera, Mesh::TRIANGLE_FAN);
}
