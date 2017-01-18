#include "../pch.h"
#pragma once

class Elevator;
class Floor;
class Rider;

struct Stop {

	Stop(shared_ptr<Floor> stopFloor, bool riderGoingUp, bool stopIsDropOff)
		: floor(stopFloor), goingUp(riderGoingUp), dropOff(stopIsDropOff) {
	}
	~Stop() {
	}
	shared_ptr<Floor> floor;
	bool goingUp;
	bool dropOff;

};


class Elevator {
public:
	Elevator(const Vector2& shaftTop, unsigned short numFloors);
	~Elevator();

	void setFloors(vector<shared_ptr<Floor>> floors);

	void update(double deltaTime);
	void draw(SpriteBatch* batch);


	void callElevatorTo(USHORT newFloorToQueue, bool goingUp);
	void selectFloor(USHORT floorRequested, bool riderGoingUp);

	bool hasNextStop();
	void doorsClosed();

	const Vector2& getShaftPosition() const;
	const Vector2& getCarPosition() const;

	const int getWidth() const;

	enum ElevatorState {
		GoingDown, GoingUp, Waiting, DoorsOpening, DoorsClosing, Loading
	};

	ElevatorState state = Waiting;
private:

	
	unique_ptr<RectangleFrame> shaft;
	unique_ptr<RectangleFrame> car;

	// This array corresponds to CallButtons that have been pushed
	//bool* floorCalls;
	vector<shared_ptr<Floor>> floors;

	/* Current going to list. */
	//list<shared_ptr<Floor> > stopQueue;
	list<shared_ptr<Stop>> stopQueue;

	/* Floors with riders waiting to go up that IS NOT in the stopQueue. */
	//list<shared_ptr<Floor> > upQueue;
	list<shared_ptr<Stop>> upQueue;
	/* Floors with riders waiting to go down that IS NOT in the stopQueue. */
	//list<shared_ptr<Floor> > downQueue;
	list<shared_ptr<Stop>> downQueue;


	// use LERP for movement between floors!
	//float currentLocation = 0;
	/* The current floor that the car is travelling through. */
	vector<shared_ptr<Floor>>::iterator currentFloor;

	//shared_ptr<Floor> nextStop = NULL;
	shared_ptr<Stop> nextStop = NULL;

	// tells elevator to get ready to move
	//void wakeUp(shared_ptr<Floor> nextFloor);

	Vector2 shaftTop;

	float moveSpeed = 50;

	vector<Rider*> ridersRiding;

};




// When going up to a destination
	// check each floor in between to see if rider is waiting to go same direction