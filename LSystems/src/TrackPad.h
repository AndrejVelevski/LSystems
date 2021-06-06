#pragma once

#include "Utils/Types.h"
#include "Renderer/ICamera.h"
#include "Renderer/Mesh.h"

#include <glm/glm.hpp>

class TrackPad
{
public:
	TrackPad(float width, float height, float x, float y);
	~TrackPad();

	glm::vec2 getTrackerPosition();
	void setTrackerPosition(glm::vec2 position);
	bool contains(float x, float y);
	void setColor(glm::vec3 color);

	void draw(ICamera* camera);

private:
	Mesh* mMeshPad;
	Mesh* mMeshTracker;

	float mWidth;
	float mHeight;
};

