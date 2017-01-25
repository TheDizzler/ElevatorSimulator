#include "../pch.h"
#pragma once

#include "Elevator.h"
#include "../Managers/GFXAssetManager.h"

/** Buttons at each floor outside of elevator to call the elevator to pick up a rider.
	Usually has two buttons: up and down. */
class CallButtons : public Sprite {
public:
	CallButtons(bool hasUpButton = true, bool hasDownButton = true);
	~CallButtons();

	bool upButtonPressed = false;
	bool downButtonPressed = false;

	void pushUpButton();
	void pushDownButton();

	void elevatorArrivedGoingUp();
	void elevatorArivedGoingDown();

private:

	Elevator* elevator;

	// call button sprites
	shared_ptr<AssetSet> gfxSet;

};