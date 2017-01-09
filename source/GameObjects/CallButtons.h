#include "../pch.h"
#pragma once

#include "Elevator.h"

/** Buttons at each floor outside of elevator to call the elevator to pick up a rider.
	Usually has two buttons: up and down. */
class CallButtons {
public:
	CallButtons(bool hasUpButton = true, bool hasDownButton = true);
	~CallButtons();

	void callElevatorToThisFloor(bool goingUp);

private:

	bool upButtonPressed = false;
	bool downButtonPressed = false;

	Elevator* elevator;

	// call button sprites
	/*unique_ptr<Sprite> offoff;
	unique_ptr<Sprite> offon;
	unique_ptr<Sprite> onoff;
	unique_ptr<Sprite> onon;*/

};