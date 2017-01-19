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

	riderState = RiderState::EnteringElevator;
	elevator = awaitingElevator;
	originalPosition = sprite->getPosition();
	wayPoint = Vector2(elevator->getCarPosition().x + elevator->getWidth() / 2, originalPosition.y);


}


void Rider::exitElevator(shared_ptr<Floor> floor) {
	currentFloor = floor;
	setWaypoint();
}


void Rider::moveBy(const Vector2& moveAmount) {
	sprite->moveBy(moveAmount);
}


double timeTravelling = 0;
Vector2 direction;
void Rider::update(double deltaTime) {

	switch (riderState) {
		case GoingToElevator:
		{
			int buttonLoc = currentFloor->callButtonLocation();
			Vector2 pos = sprite->getPosition();
			if (abs(buttonLoc - pos.x) <= 15) {
				riderState = WaitingForElevator;
				currentFloor->pushUpButton(this);
			} else {

				Vector2 newpos = pos;
				Vector2 dir = Vector2(buttonLoc - pos.x, 0);
				dir.Normalize();
				newpos += dir*moveSpeed*deltaTime;
				sprite->setPosition(newpos);

			}
			break;
		}
		case EnteringElevator:
		{

			timeTravelling += deltaTime;
			sprite->setPosition(Vector2::Lerp(originalPosition, wayPoint, 2 * timeTravelling));
			if (abs(sprite->getPosition().x - wayPoint.x) <= 10) {
				riderState = RiderState::InElevator;
				elevator->enterElevator(this);
				currentFloor.reset();
				//elevator->selectFloor(finalDestination, currentFloor->floorNumber < finalDestination);
			}


			break;
		}
		case GoingToDestination:
		{
			sprite->moveBy(direction*moveSpeed*deltaTime);
			if (sprite->getHitArea()->collision(finalDestination->getHitArea())) {
				++finalDestination->numRidersExited;
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


void Rider::setWaypoint() {

	if (finalDestination->floorNumber != currentFloor->floorNumber) {
		// set path to CallButton
		wayPoint = sprite->getPosition();
		wayPoint.x = currentFloor->callButtonLocation();
		riderState = GoingToElevator;
	} else {
		// set path to destination room
		wayPoint = finalDestination->getPosition();
		direction = wayPoint - sprite->getPosition();
		direction.Normalize();
		riderState = RiderState::GoingToDestination;

	}


}
