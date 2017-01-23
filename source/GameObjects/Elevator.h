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

	//vector<Rider*> ridersDisembarking(USHORT floorNumber);
	void startUnloading();
	bool stillUnloading();
	void doneTransferring();

	shared_ptr<Floor> getCurrentFloor();

	bool hasNextStop();
	void doorsClosed();

	const Vector2& getShaftPosition() const;
	const Vector2& getCarPosition() const;

	const int getWidth() const;

	enum ElevatorState {
		GoingDown, GoingUp, Idle, /*WaitingForDoors,*/ Transferring/*, DoorsOpening, DoorsClosing, Loading*/
	};

	ElevatorState state = Idle;

	/* Time between "waves" of riders disembarking/loading. */
	const double TIME_BETWEEN_TRANSFER = 1.0;
	/* Maximum amount of riders that can enter or exit an elevator simultaneously. */
	const USHORT MAX_RIDERS_TRANSFERRING = 2;
private:

	bool nextStopDirectionUp();
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

	vector<Rider*> disembarking;
	double timeUntilNextTransfer = 0;

	
	

	shared_ptr<Stop> nextStop = NULL;


	Vector2 shaftTop;

	float moveSpeed = 50;

	list<Rider*> ridersRiding;

};




// When going up to a destination
	// check each floor in between to see if rider is waiting to go same direction