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


	void pushUpButton();
	void pushDownButton();

	void elevatorArrivedGoingUp();
	void elevatorArivedGoingDown();

private:

	bool upButtonPressed = false;
	bool downButtonPressed = false;

	Elevator* elevator;

	// call button sprites
	shared_ptr<AssetSet> gfxSet;
	/*unique_ptr<Sprite> offoff;
	unique_ptr<Sprite> offon;
	unique_ptr<Sprite> onoff;
	unique_ptr<Sprite> onon;*/

};