#include "Elevator.h"


#include "../Engine/GameEngine.h"
#include "Building.h"
Elevator::Elevator(const Vector2& top, unsigned short numFloors) {

	shaftTop = top;

	Vector2 shaftDimensions = Vector2(BuildingData::SHAFT_WIDTH, BuildingData::FLOOR_HEIGHT*numFloors);
	shaft.reset(guiFactory->createRectangleFrame(
		shaftTop, shaftDimensions, BuildingData::SHAFT_WALL_THICKNESS, Color(.5, 0, .5, 1)));



}

Elevator::~Elevator() {

	floors.clear();
	downQueue.clear();
	upQueue.clear();
	stopQueue.clear();
	ridersRiding.clear();
}

void Elevator::setFloors(vector<shared_ptr<Floor>> flrs) {

	floors = flrs;

	Vector2 carDimensions = Vector2(BuildingData::SHAFT_WIDTH - BuildingData::SHAFT_WALL_THICKNESS * 2,
		BuildingData::FLOOR_HEIGHT / 2);
	currentFloor = floors.end();
	Vector2 carPos = (*--currentFloor)->position;
	carPos.x = shaft->getPosition().x + (BuildingData::SHAFT_WIDTH - carDimensions.x) / 2;
	car.reset(guiFactory->createRectangleFrame(carPos, carDimensions));
	car->setOrigin(Vector2(0, carDimensions.y));
	wostringstream wss;
	wss << (*currentFloor)->floorNumber;
	guiOverlay->updateFloorDisplay(wss.str());

}


void Elevator::update(double deltaTime) {

	switch (state) {
		case Idle:
			// do nothing ?
			break;

		case Transferring:
			if (disembarking.size() > 0) {

				timeUntilNextTransfer -= deltaTime;
				if (timeUntilNextTransfer <= 0) {

					timeUntilNextTransfer = TIME_BETWEEN_TRANSFER;
					for (int i = 0; i < MAX_RIDERS_TRANSFERRING && i < disembarking.size(); ++i) {

						Rider* rider = disembarking.back();
						disembarking.pop_back();
						rider->exitElevator((*currentFloor));
					}
				}
			} else {
				timeUntilNextTransfer = 0;
			}
			break;

		case GoingDown:
		{
			// check if doors are closed
			if ((*currentFloor)->doorsOpen())
				break; // wait until closed
			Vector2 direction = Vector2(0, nextStop->floor->position.y - getCarPosition().y);
			direction.Normalize();
			Vector2 moveBy = direction*moveSpeed*deltaTime;
			car->moveBy(moveBy);
			for each (Rider* rider in ridersRiding)
				rider->moveBy(moveBy);

			if ((*currentFloor) == nextStop->floor && abs(car->getPosition().y - nextStop->floor->position.y) < 2) {

				bool pickingUp = nextStop->pickUp;

				nextStop->floor->elevatorArrived(nextStop->goingUp);
				state = ElevatorState::Transferring;
				stopQueue.remove(nextStop);
				nextStop.reset();

				if (stopQueue.size() == 0 && !pickingUp) {
					nextStopDirection = NextStopDirection::None;
					if (upQueue.size() > 0) {
						upQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
							return a->floor->floorNumber < b->floor->floorNumber; });
						stopQueue = upQueue;
						upQueue.clear();
						guiOverlay->updateUpQueue(upQueue);
						if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
							nextStopDirection = NextStopDirection::Up;
						else
							nextStopDirection = NextStopDirection::Down;
					} else if (downQueue.size() > 0) {
						downQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
							return a->floor->floorNumber > b->floor->floorNumber; });
						stopQueue = downQueue;
						downQueue.clear();
						guiOverlay->updateDownQueue(downQueue);
						if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
							nextStopDirection = NextStopDirection::Up;
						else
							nextStopDirection = NextStopDirection::Down;
					}
					(*currentFloor)->elevatorApproaching(nextStopDirection);
				}

				guiOverlay->updateStopQueue(stopQueue);
				guiOverlay->updateNextStopDisplay(L"-");

			} else if (car->getPosition().y > (*currentFloor)->position.y) {
				--currentFloor;
				wostringstream wss;
				wss << (*currentFloor)->floorNumber;
				guiOverlay->updateFloorDisplay(wss.str());

				if ((*currentFloor) == nextStop->floor) {

					if (stopQueue.size() <= 1 && !nextStop->pickUp) {
						nextStopDirection = NextStopDirection::None;
						if (upQueue.size() > 0) {
							stopQueue = upQueue;
							upQueue.clear();
							guiOverlay->updateUpQueue(upQueue);
							nextStopDirection = NextStopDirection::Up;
						} else if (downQueue.size() > 0) {
							stopQueue = downQueue;
							downQueue.clear();
							guiOverlay->updateDownQueue(downQueue);
							nextStopDirection = NextStopDirection::Down;
						}
					} else if (nextStop->goingUp)
						nextStopDirection = NextStopDirection::Up;
					else
						nextStopDirection = NextStopDirection::Down;
					nextStop->floor->elevatorApproaching(nextStopDirection);
				}
			}
			break;
		}
		case GoingUp:
		{
			// check if doors are closed
			if ((*currentFloor)->doorsOpen())
				break; // wait until closed
			Vector2 direction = Vector2(0, nextStop->floor->position.y - getCarPosition().y);
			direction.Normalize();
			Vector2 moveBy = direction*moveSpeed*deltaTime;
			car->moveBy(moveBy);
			for each (Rider* rider in ridersRiding)
				rider->moveBy(moveBy);

			if ((*currentFloor) == nextStop->floor && abs(car->getPosition().y - nextStop->floor->position.y) < 2) {
			// arrived at destination
				bool pickingUp = nextStop->pickUp;
				nextStop->floor->elevatorArrived(nextStop->goingUp);
				state = ElevatorState::Transferring;
				stopQueue.remove(nextStop);
				nextStop.reset();

				if (stopQueue.size() == 0 && !pickingUp) {
					nextStopDirection = NextStopDirection::None;
					if (downQueue.size() > 0) {
						downQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
							return a->floor->floorNumber > b->floor->floorNumber; });
						stopQueue = downQueue;
						downQueue.clear();
						guiOverlay->updateDownQueue(downQueue);
						if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
							nextStopDirection = NextStopDirection::Up;
						else
							nextStopDirection = NextStopDirection::Down;
					} else if (upQueue.size() > 0) {
						upQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
							return a->floor->floorNumber < b->floor->floorNumber; });
						stopQueue = upQueue;
						upQueue.clear();
						guiOverlay->updateUpQueue(upQueue);
						if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
							nextStopDirection = NextStopDirection::Up;
						else
							nextStopDirection = NextStopDirection::Down;
					}
					(*currentFloor)->elevatorApproaching(nextStopDirection);
				}
				guiOverlay->updateStopQueue(stopQueue);
				guiOverlay->updateNextStopDisplay(L"-");

			} else if (car->getPosition().y - car->getHeight() <
				(*currentFloor)->position.y - BuildingData::FLOOR_HEIGHT) {

				++currentFloor;
				wostringstream wss;
				wss << (*currentFloor)->floorNumber;
				guiOverlay->updateFloorDisplay(wss.str());

				if ((*currentFloor) == nextStop->floor) {
					if (stopQueue.size() - 1 == 0 && !nextStop->pickUp) {

						nextStopDirection = NextStopDirection::None;
						if (downQueue.size() > 0) {
							stopQueue = downQueue;
							downQueue.clear();
							guiOverlay->updateDownQueue(downQueue);
							if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
								nextStopDirection = NextStopDirection::Up;
							else
								nextStopDirection = NextStopDirection::Down;
						} else if (upQueue.size() > 0) {
							stopQueue = upQueue;
							upQueue.clear();
							guiOverlay->updateUpQueue(upQueue);
							if (stopQueue.front()->floor->floorNumber > (*currentFloor)->floorNumber)
								nextStopDirection = NextStopDirection::Up;
							else
								nextStopDirection = NextStopDirection::Down;
						}
					} else if (nextStop->goingUp)
						nextStopDirection = NextStopDirection::Up;
					else
						nextStopDirection = NextStopDirection::Down;
					nextStop->floor->elevatorApproaching(nextStopDirection);
				}
			}
			break;
		}

	}


}


void Elevator::draw(SpriteBatch* batch) {

	car->draw(batch);
	shaft->draw(batch);


}


//bool Elevator::nextStopDirectionUp() {
//	return nextStop->floor->floorNumber > (*currentFloor)->floorNumber;
//}


void Elevator::callElevatorTo(USHORT newFloorNumberToQueue, bool riderGoingUp) {

	shared_ptr<Stop> newStop = make_shared<Stop>(floors[newFloorNumberToQueue - 1], riderGoingUp, false, true);

	switch (state) {
		case Idle:
			if (newStop->floor->floorNumber > (*currentFloor)->floorNumber) {
				state = GoingUp;
				nextStop = newStop;
				stopQueue.push_front(move(newStop));
				wostringstream wssNext;
				wssNext << nextStop->floor->floorNumber;
				guiOverlay->updateNextStopDisplay(wssNext.str());
			} else if (newStop->floor->floorNumber < (*currentFloor)->floorNumber) {
				state = GoingDown;
				nextStop = newStop;
				stopQueue.push_front(move(newStop));
				wostringstream wssNext;
				wssNext << nextStop->floor->floorNumber;
				guiOverlay->updateNextStopDisplay(wssNext.str());
			} else {
				state = ElevatorState::Transferring;
				newStop->floor->elevatorArrived(riderGoingUp);
			}
			break;

		case WaitingForDoors:
		case Transferring:

			if (riderGoingUp) {
				if (nextStopDirection == NextStopDirection::Up
					&& newStop->floor->floorNumber > (*currentFloor)->floorNumber) {
						 // add to current queue
					stopQueue.push_front(move(newStop));
					stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber < b->floor->floorNumber; });
				} else {
					// add to UpQueue
					upQueue.push_front(move(newStop));
				}
			} else {
				if (nextStopDirection == NextStopDirection::Down
					&& newStop->floor->floorNumber < (*currentFloor)->floorNumber) {
						// add to current queue
					stopQueue.push_front(move(newStop));
					stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber > b->floor->floorNumber; });

				} else {
					// add to downQueue
					downQueue.push_front(move(newStop));
				}
			}
			break;

		case GoingUp:	// if car going up
			if (riderGoingUp) {
			// if car is below rider location
				if (newStop->floor->position.y < car->getPosition().y) {
					// add to queue and sort
					stopQueue.push_front(move(newStop));
					stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber < b->floor->floorNumber; });
				} else { // missed it...
					// rider below car: place in up queue for later
					upQueue.push_front(move(newStop));
				}
			} else {
				// place in to down queue
				downQueue.push_front(move(newStop));
			}
			break;

		case GoingDown:	// if car going down
			if (!riderGoingUp) {	// and rider is going down
			// if car is ABOVE rider location
				if (newStop->floor->position.y > car->getPosition().y) {
					// add to queue and sort
					stopQueue.push_front(move(newStop));
					stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber > b->floor->floorNumber; });
				} else { // missed it...
					downQueue.push_front(move(newStop));
				}
			} else {
				// rider below car: place in up queue for later
				upQueue.push_front(move(newStop));
			}

			break;
	}

	guiOverlay->updateStopQueue(stopQueue);
	guiOverlay->updateUpQueue(upQueue);
	guiOverlay->updateDownQueue(downQueue);
	if (nextStop.get() != NULL) {
		wostringstream wssNext;
		wssNext << nextStop->floor->floorNumber;
		guiOverlay->updateNextStopDisplay(wssNext.str());
	}
}


void Elevator::enterElevator(Rider* rider) {

	selectFloor(rider->finalDestination->floorNumber,
		rider->currentFloor->floorNumber < rider->finalDestination->floorNumber);
	ridersRiding.push_back(rider);
}


void Elevator::startUnloading() {

	for (Rider* rider : ridersRiding)
		if (rider->finalDestination->floorNumber == (*currentFloor)->floorNumber)
			disembarking.push_back(rider);

	for (Rider* rider : disembarking)
		ridersRiding.remove(rider);
}

bool Elevator::stillUnloading() {
	return disembarking.size() > 0;
}

void Elevator::doneTransferring() {
	//state = ElevatorState::WaitingForDoors;
	//if (nextStop.get() == NULL) {
	//if (stopQueue.size() != 0)

	if (stopQueue.size() == 0 && downQueue.size() == 0 && upQueue.size() == 0)
		state = ElevatorState::Idle;
	else
		state = Elevator::WaitingForDoors;
/*} else {

	if (nextStop->floor->floorNumber > (*currentFloor)->floorNumber)
		state = ElevatorState::GoingUp;
	else
		state = ElevatorState::GoingDown;
}*/
}

shared_ptr<Floor> Elevator::getCurrentFloor() {
	return *currentFloor;
}


void Elevator::selectFloor(USHORT floorRequested, bool riderGoingUp) {

	for (list<shared_ptr<Stop>>::iterator it = stopQueue.begin(); it != stopQueue.end(); ++it) {
		if ((*it)->floor->floorNumber == floorRequested)
			return;
	}
	stopQueue.push_front(make_shared<Stop>(floors[floorRequested - 1], riderGoingUp, true, false));
	stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
		return a->floor->floorNumber < b->floor->floorNumber; });

	guiOverlay->updateStopQueue(stopQueue);

}


bool Elevator::hasNextStop() {
	/*if (stopQueue.size() > 0)
		return true;
	if (downQueue.size() > 0) {
		stopQueue = downQueue;
		downQueue.clear();
		return true;
	}*/
	return stopQueue.size() > 0;
}

void Elevator::doorsClosed() {

	nextStop = stopQueue.front();
	if (nextStop->floor->floorNumber > (*currentFloor)->floorNumber)
		state = ElevatorState::GoingUp;
	else
		state = ElevatorState::GoingDown;

	wostringstream wssNext;
	wssNext << nextStop->floor->floorNumber;
	guiOverlay->updateNextStopDisplay(wssNext.str());
}


const Vector2& Elevator::getShaftPosition() const {

	return shaftTop;
}

const Vector2& Elevator::getCarPosition() const {

	return car->getPosition();
}

const int Elevator::getWidth() const {
	return shaft->getWidth();
}
