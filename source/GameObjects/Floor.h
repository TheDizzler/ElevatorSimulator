#include "../pch.h"
#pragma once

#include "CallButtons.h"

class Floor {
public:
	Floor(USHORT floorNum, Vector2 floorPosition);
	~Floor();


	void update(double deltaTime);
	void draw(SpriteBatch* batch);
private:

	unique_ptr<Line> line; // this is the floor
	unique_ptr<CallButtons> callButtons;

	USHORT floorNumber;

};