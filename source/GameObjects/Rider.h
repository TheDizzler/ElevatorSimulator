#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/Sprite.h"
#include "Floor.h"

/* A simulant designed for riding elevators. */
class Rider {
public:
	Rider(unsigned short destinationFloor);
	~Rider();

	void setSprite(GraphicsAsset* gfxAsset);
	void setFloor(Floor* floor);
	//void setDestinationFloor(unsigned short destination);

	unsigned short destinationFloor;


	void draw(SpriteBatch* batch);

private:

	unique_ptr<Sprite> sprite;

	Floor* currentFloor;
};
