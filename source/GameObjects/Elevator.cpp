#include "Elevator.h"

#include "../Engine/GameEngine.h"
#include "Building.h"
Elevator::Elevator(const Vector2& top, unsigned short numFloors) {

	shaftTop = top;

	/*floorCalls = new bool[numFloors];
	for (int i = 0; i < numFloors; ++i)
		floors[i] = false;*/

	Vector2 shaftDimensions = Vector2(BuildingData::SHAFT_WIDTH, BuildingData::FLOOR_HEIGHT*numFloors);
	shaft.reset(GameManager::guiFactory->createRectangleFrame(
		shaftTop, shaftDimensions, BuildingData::SHAFT_WALL_THICKNESS, Color(.5, 0, .5, 1)));

	Vector2 carDimensions = Vector2(BuildingData::SHAFT_WIDTH - BuildingData::SHAFT_WALL_THICKNESS * 2,
		BuildingData::FLOOR_HEIGHT / 2);
	Vector2 carPos = shaftTop;
	carPos.x += (BuildingData::SHAFT_WIDTH - carDimensions.x) / 2;
	car.reset(GameManager::guiFactory->createRectangleFrame(carPos, carDimensions));


	queueListDialog.reset(GameManager::guiFactory->createDialog(true));
	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2);
	dialogPos = dialogSize;
	dialogPos.x -= dialogSize.x / 2;
	dialogPos.y -= dialogSize.y / 2;
	queueListDialog->setDimensions(dialogPos, dialogSize);
	queueListDialog->open();

}

Elevator::~Elevator() {
	//delete[] floorCalls;
	floors.clear();
}

void Elevator::setFloors(vector<shared_ptr<Floor>> flrs) {

	floors = flrs;
		//for each (shared_ptr<Floor> floor in floors)
			//floorMap[floor] = false;
}


void Elevator::update(double deltaTime) {

	switch (state) {
		case Waiting:
			// do nothing ?
			break;


	}
	queueListDialog->update(deltaTime);
}


void Elevator::draw(SpriteBatch* batch) {

	car->draw(batch);
	shaft->draw(batch);

	queueListDialog->draw(batch);
}



void Elevator::callElevatorTo(USHORT newFloorNumberToQueue, bool riderGoingUp) {

	//floorCalls[newFloorToQueue->floorNumber] = true;

	shared_ptr<Floor> nextFloor = floors[newFloorNumberToQueue];
	/*for each (shared_ptr<Floor> floor in floors) {
		if (floor.get() == newFloorToQueue) {
			nextFloor = floor;
			break;
		}
	}*/


	switch (state) {
		case Waiting:

			//wakeUp(floor);
			nextStop = nextFloor;
			stopQueue.push_front(nextFloor);
			break;
		case GoingUp:	// if car going up
			if (riderGoingUp) {
			// if car is below rider location
				if (nextFloor->position.y < car->getPosition().y) {
					// add to appriopriate place in queue
					/*for each (shared_ptr<Floor> floor in upList) {
						if (floor->floorNumber > nextFloor->floorNumber) {


						}

					}*/

					// add to queue and sort
					stopQueue.push_front(nextFloor);
					stopQueue.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber < b->floorNumber; });
				} else { // missed it...
					// rider below car: place in up queue for later
					upList.push_front(nextFloor);
					upList.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber < b->floorNumber; });
				}
			} else {
				// place in to down queue
				downList.push_front(nextFloor);
				downList.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
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
					downList.push_front(nextFloor);
					downList.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
						return a->floorNumber > b->floorNumber; });
				}
			} else {
				// rider below car: place in up queue for later
				upList.push_front(nextFloor);
				upList.sort([](const shared_ptr<Floor> a, const shared_ptr<Floor> b) {
					return a->floorNumber < b->floorNumber; });

			}

			break;
	}

	wostringstream wss;
	wss << "StopQueue \n";
	for each (shared_ptr<Floor> floor in stopQueue)
		wss << floor->floorNumber << "\n";
	queueListDialog->setText(wss.str());
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
