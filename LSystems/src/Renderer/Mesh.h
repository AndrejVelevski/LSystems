#pragma once

#include "Shader.h"
#include "Transform.h"
#include "ICamera.h"
#include "../Utils/Types.h"

#include <vector>

class Mesh : public Transform
{
public:
	Mesh();
	Mesh(const std::vector<float>& vertices, const std::vector<uint32>& elements);

	void bind() const;
	void unbind() const;

	void update(float delta);
	void draw(Shader& shader, const ICamera& camera) const;

private:
	uint32 mVAO;
	uint32 mVBO;
	uint32 mEBO;

	std::vector<float> mVertices;
	std::vector<uint32> mElements;
};

