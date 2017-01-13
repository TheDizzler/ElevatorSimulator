#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/Sprite.h"
#include "Floor.h"

/* A simulant designed for riding elevators. */
class Rider {
public:
	Rider(GraphicsAsset* gfxAsset, shared_ptr<Floor> currentFloor, unsigned short destinationFloor);
	~Rider();

	

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

private:
	enum RiderState {
		GoingToElevator, WaitingForElevator, GoingInElevator, InElevator, ExitingElevator, GoingToDestination
	};
	RiderState riderState = GoingToElevator;

	unique_ptr<Sprite> sprite;

	Vector2 currentDestination;

	unsigned short finalDestination;
	shared_ptr<Floor> currentFloor;

	double timeAlive = 0;
	double timeWaiting = 0;
	double timeInElevator = 0;
	double timeAfterLeaving = 0;

	float moveSpeed = 100;


	void setFloor(shared_ptr<Floor> floor);
	void setDestinationFloor(unsigned short destination);
};
