#include "../pch.h"
#pragma once

#include "Floor.h"
#include "Rider Bubble.h"

/* A simulant designed for riding elevators. It's only function is to use an elevator to get
	to a mysterious room in a building. Or to exit the building never to be seen again... */
class Rider {
public:
	Rider(GraphicsAsset* gfxAsset, shared_ptr<Floor> startFloor, Exit* startExit, shared_ptr<Exit> destinationExit);
	~Rider();

	void enterElevator(Elevator* awaitingElevator);
	void exitElevator(shared_ptr<Floor> floor);
	void moveBy(const Vector2& moveAmount);

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	
	shared_ptr<Exit> finalDestination;
	shared_ptr<Floor> currentFloor;

	enum RiderState {
		GoingToElevator, WaitingForElevator, EnteringElevator,
		InElevator, ExitingElevator, GoingToDestination, Exited
	};
	RiderState riderState = GoingToElevator;

private:
	

	unique_ptr<Sprite> sprite;

	double timeTravelling = 0;
	Vector2 direction;

	Vector2 waypoint;


	Elevator* elevator = NULL;

	double timeAlive = 0;
	double timeWaiting = 0;
	double timeInElevator = 0;
	double timeAfterLeaving = 0;

	float moveSpeed = 100;

	void setWaypoint();

	unique_ptr<RiderBubble> thoughtBubble;

};
