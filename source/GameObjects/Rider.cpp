#include "Rider.h"

Rider::Rider(GraphicsAsset* gfxAsset, shared_ptr<Floor> startFloor,
	Exit* startExit, shared_ptr<Exit> destinationExit) {

	sprite.reset(new Sprite());
	sprite->load(gfxAsset);

	//setFloor(floor);
	currentFloor = startFloor;
	Vector2 pos = startExit->getPosition();
	pos.x -= sprite->getWidth() / 2;
	pos.y -= sprite->getHeight() / 2;
	sprite->setPosition(pos);
	finalDestination = destinationExit;
	setWaypoint();

}

Rider::~Rider() {
}

void Rider::enterElevator(Elevator* awaitingElevator) {

	elevator = awaitingElevator;

	riderState = RiderState::EnteringElevator;
	waypoint = sprite->getPosition();
	waypoint.x = elevator->getCarPosition().x + elevator->getWidth() / 2;
	direction = waypoint - sprite->getPosition();
	direction.Normalize();


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

#include "../Engine/GameEngine.h"
void Rider::update(double deltaTime) {

	/*wostringstream wss;
	wss << "x: " << sprite->getPosition().x << " , y: " << sprite->getPosition().y;
	guiOverlay->testLabel->setText(wss);*/

	switch (riderState) {
		case GoingToElevator:
		{
			if ((sprite->getPosition().x - waypoint.x) * direction.x >= 10) {
				riderState = WaitingForElevator;
				if (currentFloor->floorNumber < finalDestination->floorNumber) {
					if (currentFloor->elevatorGoingUpDoorOpen()) // if up elevator already here
						currentFloor->getInElevator(this);
					else
						currentFloor->pushUpButton(this);
				} else {
					if (currentFloor->elevatorGoingDownDoorOpen()) // if up elevator already here
						currentFloor->getInElevator(this);
					else
						currentFloor->pushDownButton(this);
				}
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


//void Rider::setFloor(shared_ptr<Floor> floor) {
//
//	currentFloor = floor;
//	Vector2 pos = floor->position;
//	pos.x -= sprite->getWidth() / 2;
//	pos.y -= sprite->getHeight() / 2;
//	sprite->setPosition(pos);
//
//}



