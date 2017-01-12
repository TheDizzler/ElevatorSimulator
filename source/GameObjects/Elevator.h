#include "../pch.h"
#pragma once

enum State { GOING_DOWN, GOING_UP, WAITING};

class Elevator {
public:
	Elevator(const Vector2& shaftTop, unsigned short numFloors);
	~Elevator();

	void draw(SpriteBatch* batch);
	void update(double deltaTime);

	void callElevatorTo(unsigned short destinationFloor); 

	const Vector2& getPosition() const;
	const int getWidth() const;
private:

	unique_ptr<RectangleFrame> shaft;
	unique_ptr<RectangleFrame> car;

	// when a floor is TRUE the elevator will stop there
	bool* floors;

	State state = WAITING;

	// use LERP for movement between floors!
	float currentLocation = 0;

	// tells elevator to get ready to move
	void wakeUp();

	Vector2 shaftTop;

};