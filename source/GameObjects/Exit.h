#include "../pch.h"
#pragma once


class Exit {
public:
	Exit(USHORT floorNumber);
	~Exit();

	void setPosition(const Vector2& position);

	const Vector2& getPosition();
	const int getWidth();
	const int getHeight();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	USHORT floorNumber;

private:

	unique_ptr<Sprite> door;

	UINT numRidersExited = 0;
	UINT numRuidersGenerated = 0;

};