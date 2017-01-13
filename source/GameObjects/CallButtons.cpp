#include "CallButtons.h"

#include "../Managers/GameManager.h"
CallButtons::CallButtons(bool hasUpButton, bool hasDownButton) : Sprite() {

	gfxSet = GameManager::gfxAssets->getAssetSet("Call Buttons");
	load(gfxSet->getAsset("Call Buttons Off/Off"));
}

CallButtons::~CallButtons() {
}

void CallButtons::pushUpButton() {

	// change lights
		load(gfxSet->getAsset("Call Buttons On/Off"));

}

