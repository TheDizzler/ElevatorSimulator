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

	const HitArea* getHitArea();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	USHORT floorNumber;

	UINT numRidersExited = 0;
	UINT numRuidersGenerated = 0;

private:

	unique_ptr<Sprite> door;

	

};