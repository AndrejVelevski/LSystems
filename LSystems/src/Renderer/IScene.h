#pragma once

class IScene
{
public:
	virtual void update(float delta) const = 0;
	virtual void draw() const = 0;
};

