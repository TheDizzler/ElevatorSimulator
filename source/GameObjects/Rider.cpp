#include "Rider.h"

Rider::Rider(GraphicsAsset* gfxAsset, shared_ptr<Floor> floor, unsigned short destination) {

	sprite.reset(new Sprite());
	sprite->load(gfxAsset);

	setFloor(floor);
	setDestinationFloor(destination);

}

Rider::~Rider() {
}


void Rider::setFloor(shared_ptr<Floor> floor) {

	currentFloor = floor;
	Vector2 pos = floor->position;
	pos.x -= sprite->getWidth() / 2;
	pos.y -= sprite->getHeight() / 2;
	sprite->setPosition(pos);

}

void Rider::setDestinationFloor(unsigned short destination) {

	finalDestination = destination;
	if (finalDestination != currentFloor->floorNumber) {
		// setPathTo callButton
		currentDestination = sprite->getPosition();
		currentDestination.x = currentFloor->callButtonLocation();
		riderState = GoingToElevator;
	} else {
		// set path to destination room

	}
}

void Rider::update(double deltaTime) {

	switch (riderState) {
		case GoingToElevator:
			int buttonLoc = currentFloor->callButtonLocation();
			Vector2 pos = sprite->getPosition();
			if (abs(buttonLoc - pos.x) <= 15) {
				riderState = WaitingForElevator;
				currentFloor->pushUpButton();
			} else {

				Vector2 newpos = pos;
				Vector2 dir = Vector2(buttonLoc-pos.x, 0);
				dir.Normalize();
				newpos += dir*moveSpeed*deltaTime;
				sprite->setPosition(newpos);
				break;
			}


	}
}

void Rider::draw(SpriteBatch* batch) {
	sprite->draw(batch);
}
