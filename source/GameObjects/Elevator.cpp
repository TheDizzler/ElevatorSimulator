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
		case Waiting:
			// do nothing ?
			break;
		case GoingDown:
		{
			Vector2 direction = Vector2(0, nextStop->floor->position.y - getCarPosition().y);
			direction.Normalize();
			Vector2 moveBy = direction*moveSpeed*deltaTime;
			car->moveBy(moveBy);
			for each (Rider* rider in ridersRiding)
				rider->moveBy(moveBy);

			if ((*currentFloor) == nextStop->floor && abs(car->getPosition().y - nextStop->floor->position.y) < 2) {

				nextStop->floor->elevatorArrived(nextStop->goingUp);
				state = ElevatorState::DoorsOpening;
				stopQueue.remove(nextStop);
				nextStop.reset();
				guiOverlay->updateStopQueue(stopQueue);
				guiOverlay->updateNextStopDisplay(L"-");

			} else if (car->getPosition().y > (*currentFloor)->position.y) {
				--currentFloor;
				wostringstream wss;
				wss << (*currentFloor)->floorNumber;
				guiOverlay->updateFloorDisplay(wss.str());

				if ((*currentFloor) == nextStop->floor) {
					NextStopDirection nsd;
					if (stopQueue.size() <= 1 && !nextStop->pickUp)
						nsd = NextStopDirection::None;
					else if (nextStop->goingUp)
						nsd = NextStopDirection::Up;
					else
						nsd = NextStopDirection::Down;
					nextStop->floor->elevatorApproaching(nsd);
				}
			}
			break;
		}
		case GoingUp:
		{
			Vector2 direction = Vector2(0, nextStop->floor->position.y - getCarPosition().y);
			direction.Normalize();
			Vector2 moveBy = direction*moveSpeed*deltaTime;
			car->moveBy(moveBy);
			for each (Rider* rider in ridersRiding)
				rider->moveBy(moveBy);

			if ((*currentFloor) == nextStop->floor && abs(car->getPosition().y - nextStop->floor->position.y) < 2) {

				nextStop->floor->elevatorArrived(nextStop->goingUp);
				state = ElevatorState::DoorsOpening;
				stopQueue.remove(nextStop);
				nextStop.reset();
				guiOverlay->updateStopQueue(stopQueue);
				guiOverlay->updateNextStopDisplay(L"-");

			} else if (car->getPosition().y - car->getHeight() < (*currentFloor)->position.y - BuildingData::FLOOR_HEIGHT) {
				++currentFloor;
				wostringstream wss;
				wss << (*currentFloor)->floorNumber;
				guiOverlay->updateFloorDisplay(wss.str());

				if ((*currentFloor) == nextStop->floor) {
					NextStopDirection nsd;
					if (stopQueue.size() <= 1 && !nextStop->pickUp)
						nsd = NextStopDirection::None;
					else if (nextStop->goingUp)
						nsd = NextStopDirection::Up;
					else
						nsd = NextStopDirection::Down;
					nextStop->floor->elevatorApproaching(nsd);
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



void Elevator::callElevatorTo(USHORT newFloorNumberToQueue, bool riderGoingUp) {

	shared_ptr<Stop> newStop = make_shared<Stop>(floors[newFloorNumberToQueue - 1], riderGoingUp, false, true);

	switch (state) {
		case Waiting:

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
				state = ElevatorState::DoorsOpening;
				newStop->floor->elevatorArrived(riderGoingUp);
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
					upQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber < b->floor->floorNumber; });
				}
			} else {
				// place in to down queue
				downQueue.push_front(move(newStop));
				downQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
					return a->floor->floorNumber > b->floor->floorNumber; });
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
					downQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
						return a->floor->floorNumber > b->floor->floorNumber; });
				}
			} else {
				// rider below car: place in up queue for later
				upQueue.push_front(move(newStop));
				upQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
					return a->floor->floorNumber < b->floor->floorNumber; });

			}

			break;
	}

	guiOverlay->updateStopQueue(stopQueue);
	guiOverlay->updateUpQueue(upQueue);
	guiOverlay->updateDownQueue(downQueue);
	wostringstream wssNext;
	wssNext << nextStop->floor->floorNumber;
	guiOverlay->updateNextStopDisplay(wssNext.str());
}

void Elevator::enterElevator(Rider* rider) {

	selectFloor(rider->finalDestination, rider->currentFloor->floorNumber < rider->finalDestination);
	rider->currentFloor.reset();
	ridersRiding.push_back(rider);
}


void Elevator::selectFloor(USHORT floorRequested, bool riderGoingUp) {

	stopQueue.push_front(make_shared<Stop>(floors[floorRequested - 1], riderGoingUp, true, false));
	stopQueue.sort([](const shared_ptr<Stop> a, const shared_ptr<Stop> b) {
		return a->floor->floorNumber < b->floor->floorNumber; });

	guiOverlay->updateStopQueue(stopQueue);

}


bool Elevator::hasNextStop() {
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
