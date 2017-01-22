#include "Rider.h"

Rider::Rider(GraphicsAsset* gfxAsset, shared_ptr<Floor> floor, shared_ptr<Exit> destination) {

	sprite.reset(new Sprite());
	sprite->load(gfxAsset);

	setFloor(floor);
	finalDestination = destination;
	setWaypoint();

}

Rider::~Rider() {
}

void Rider::enterElevator(Elevator* awaitingElevator) {

	elevator = awaitingElevator;
	//originalPosition = sprite->getPosition();
	waypoint = sprite->getPosition();
	waypoint.x = elevator->getCarPosition().x + elevator->getWidth() / 2;
	direction = waypoint - sprite->getPosition();
	direction.Normalize();
	riderState = RiderState::EnteringElevator;

}


void Rider::exitElevator(shared_ptr<Floor> floor) {
	currentFloor = floor;
	riderState = RiderState::GoingToDestination;
	setWaypoint();
}

void Rider::setWaypoint() {

	waypoint = sprite->getPosition();


	if (finalDestination->floorNumber != currentFloor->floorNumber) {
		// set path to CallButton

		waypoint.x = currentFloor->callButtonLocation();
		direction = waypoint - sprite->getPosition();
		direction.Normalize();
		riderState = RiderState::GoingToElevator;
	} else {
		// set path to destination room
		waypoint.x = finalDestination->getPosition().x;
		direction = waypoint - sprite->getPosition();
		direction.Normalize();
		riderState = RiderState::GoingToDestination;

	}


}

void Rider::moveBy(const Vector2& moveAmount) {
	sprite->moveBy(moveAmount);
}


void Rider::update(double deltaTime) {

	switch (riderState) {
		case GoingToElevator:
		{
			if ((sprite->getPosition().x - waypoint.x) * direction.x >= 10) {
				riderState = WaitingForElevator;
				currentFloor->pushUpButton(this);
			} else {
				sprite->moveBy(direction*moveSpeed*deltaTime);
			}
			break;
		}
		case EnteringElevator:
		{


			sprite->moveBy(direction*moveSpeed*deltaTime);
			if ((sprite->getPosition().x - waypoint.x) * direction.x >= 10) {
				// gone too far
				riderState = RiderState::InElevator;
				elevator->enterElevator(this);
				currentFloor.reset();
			}

			break;
		}
		case GoingToDestination:
		{
			sprite->moveBy(direction*moveSpeed*deltaTime);
			if (sprite->getHitArea()->collision(finalDestination->getHitArea())) {

				finalDestination->riderArrived(this);
				riderState = RiderState::Exited;
			}
			break;
		}
	}
}


void Rider::draw(SpriteBatch* batch) {
	sprite->draw(batch);
}


void Rider::setFloor(shared_ptr<Floor> floor) {

	currentFloor = floor;
	Vector2 pos = floor->position;
	pos.x -= sprite->getWidth() / 2;
	pos.y -= sprite->getHeight() / 2;
	sprite->setPosition(pos);

}



