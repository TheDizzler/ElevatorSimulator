#include "../pch.h"
#pragma once


class Exit {
public:
	Exit();
	~Exit();

	void setPosition(const Vector2& position);
	const int getWidth();
	const int getHeight();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

private:

	unique_ptr<Sprite> door;

	UINT numRidersExited = 0;
	UINT numRuidersGenerated = 0;

};