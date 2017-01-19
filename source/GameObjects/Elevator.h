#include "../pch.h"
#pragma once

class Elevator;
class Floor;
class Rider;

struct Stop {

	Stop(shared_ptr<Floor> stopFloor, bool riderGoingUp, bool stopIsDropOff, bool stopIsPickup)
		: floor(stopFloor), goingUp(riderGoingUp), dropOff(stopIsDropOff), pickUp(stopIsPickup) {
	}
	~Stop() {
	}
	shared_ptr<Floor> floor;
	bool goingUp;
	bool dropOff;
	bool pickUp;

};


class Elevator {
public:
	Elevator(const Vector2& shaftTop, unsigned short numFloors);
	~Elevator();

	void setFloors(vector<shared_ptr<Floor>> floors);

	void update(double deltaTime);
	void draw(SpriteBatch* batch);


	void callElevatorTo(USHORT newFloorToQueue, bool goingUp);
	void enterElevator(Rider* rider);

	vector<Rider*> ridersDisembarking(USHORT floorNumber);
	shared_ptr<Floor> getSharedFloor();

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

	void selectFloor(USHORT floorRequested, bool riderGoingUp);
	
	unique_ptr<RectangleFrame> shaft;
	unique_ptr<RectangleFrame> car;

	vector<shared_ptr<Floor>> floors;

	/* Current going to list. */
	list<shared_ptr<Stop>> stopQueue;

	/* Floors with riders waiting to go up that IS NOT in the stopQueue. */
	list<shared_ptr<Stop>> upQueue;
	/* Floors with riders waiting to go down that IS NOT in the stopQueue. */
	list<shared_ptr<Stop>> downQueue;


	// use LERP for movement between floors!
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