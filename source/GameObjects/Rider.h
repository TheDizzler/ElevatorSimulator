#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/Sprite.h"


/* A simulant designed for riding elevators. */
class Rider : public Sprite {
public:
	Rider(unsigned short destinationFloor);
	~Rider();

	//void setDestinationFloor(unsigned short destination);

	unsigned short destinationFloor;

private:


};
