#include "CallButtons.h"

#include "../Engine/GameEngine.h"
CallButtons::CallButtons(bool hasUpButton, bool hasDownButton) : Sprite() {

	gfxSet = gfxAssets->getAssetSet("Call Buttons");
	load(gfxSet->getAsset("Call Buttons Off/Off"));
}

CallButtons::~CallButtons() {
}

void CallButtons::pushUpButton() {

	// change lights
		load(gfxSet->getAsset("Call Buttons On/Off"));

}

