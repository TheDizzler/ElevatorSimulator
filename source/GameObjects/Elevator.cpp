#include "Elevator.h"

#include "../Engine/GameEngine.h"
#include "Building.h"
Elevator::Elevator(const Vector2& top, unsigned short numFloors) {

	shaftTop = top;

	/*floorCalls = new bool[numFloors];
	for (int i = 0; i < numFloors; ++i)
		floors[i] = false;*/

	Vector2 shaftDimensions = Vector2(BuildingData::SHAFT_WIDTH, BuildingData::FLOOR_HEIGHT*numFloors);
	shaft.reset(guiFactory->createRectangleFrame(
		shaftTop, shaftDimensions, BuildingData::SHAFT_WALL_THICKNESS, Color(.5, 0, .5, 1)));



}

Elevator::~Elevator() {
	//delete[] floorCalls;
	floors.clear();
}

void Elevator::setFloors(vector<shared_ptr<Floor>> flrs) {

	floors = flrs;
		//for each (shared_ptr<Floor> floor in floors)
			//floorMap[floor] = false;

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
		case GoingUp:
			Vector2 direction = Vector2(0, nextStop->position.y - getCarPosition().y);
			direction.Normalize();
			car->moveBy(direction*moveSpeed*deltaTime);

			if ((*currentFloor) == nextStop && abs(car->getPosition().y - nextStop->position.y) < 2) {

				nextStop->elevatorArrived();
				state = State::DoorsOpening;

			} else if (car->getPosition().y > (*currentFloor)->position.y) {
				--currentFloor;
				wostringstream wss;
				wss << (*currentFloor)->floorNumber;
				guiOverlay->updateFloorDisplay(wss.str());

			}
			break;

	}



}


void Elevator::draw(SpriteBatch* batch) {

	car->draw(batch);
	shaft->draw(batch);


}



void Elevator::callElevatorTo(USHORT newFloorNumberToQueue, bool riderGoingUp) {


	shared_ptr<Floor> nextFloor = floors[newFloorNumberToQueue - 1];

	switch (state) {
		case Waiting:

			//wakeUp(floor);
			if (nextFloor->position.y < getCarPosition().y) {
				state = GoingDown;
				nextStop = nextFloor;
				stopQueue.push_front(nextFloor);
			} else if (nextFloor->position.y > getCarPosition().y) {
				state = GoingUp;
				nextStop = nextFloor;
				stopQueue.push_front(nextFloor);
			} else {
				state = State::DoorsOpening;
				nextFloor->elevatorArrived();
			}


			break;
		case GoingUp:	// if car going up
			if (riderGoingUp) {
			// if car is below rider location
				if (nextFloor->position.y < car->getPosition().y) {

					// add to queue and sort
					stopQueue.push_front(nextFloor);
					stopQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber < b->floorNumber; });
				} else { // missed it...
					// rider below car: place in up queue for later
					upQueue.push_front(nextFloor);
					upQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber < b->floorNumber; });
				}
			} else {
				// place in to down queue
				downQueue.push_front(nextFloor);
				downQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
					return a->floorNumber > b->floorNumber; });
			}
			break;
		case GoingDown:	// if car going down
			if (!riderGoingUp) {	// and rider is going down
			// if car is ABOVE rider location
				if (nextFloor->position.y > car->getPosition().y) {
					// add to queue and sort
					stopQueue.push_front(nextFloor);
					stopQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber > b->floorNumber; });
				} else { // missed it...
					downQueue.push_front(nextFloor);
					downQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber > b->floorNumber; });
				}
			} else {
				// rider below car: place in up queue for later
				upQueue.push_front(nextFloor);
				upQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
					return a->floorNumber < b->floorNumber; });

			}

			break;
	}

	guiOverlay->updateStopQueue(stopQueue);
	guiOverlay->updateUpQueue(upQueue);
	guiOverlay->updateDownQueue(downQueue);
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

//void Elevator::wakeUp(shared_ptr<Floor> nextFloor) {
//
//	
//	nextStop = nextFloor;
//}
