#include "CallButtons.h"

#include "../Engine/GameEngine.h"
CallButtons::CallButtons(bool hasUpButton, bool hasDownButton) : Sprite() {

	gfxSet = gfxAssets->getAssetSet("Call Buttons");
	load(gfxSet->getAsset("Call Buttons Off/Off"));
}

CallButtons::~CallButtons() {
}

void CallButtons::pushUpButton() {

	upButtonPressed = true;
		// change lights
	if (downButtonPressed)
		load(gfxSet->getAsset("Call Buttons On/On"));
	else
		load(gfxSet->getAsset("Call Buttons On/Off"));

}

void CallButtons::pushDownButton() {

	downButtonPressed = true;
	if (upButtonPressed)
		load(gfxSet->getAsset("Call Buttons On/On"));
	else
		load(gfxSet->getAsset("Call Buttons Off/On"));
}

void CallButtons::elevatorArrivedGoingUp() {

	upButtonPressed = false;
	if (downButtonPressed)
		load(gfxSet->getAsset("Call Buttons Off/On"));
	else
		load(gfxSet->getAsset("Call Buttons Off/Off"));
}

void CallButtons::elevatorArivedGoingDown() {

	downButtonPressed = false;
	if (upButtonPressed)
		load(gfxSet->getAsset("Call Buttons On/Off"));
	else
		load(gfxSet->getAsset("Call Buttons Off/Off"));
}

