#include "Elevator.h"

#include "../Engine/GameEngine.h"
#include "Building.h"
Elevator::Elevator(const Vector2& top, unsigned short numFloors) {

	shaftTop = top;

	floors = new bool[numFloors];
	for (int i = 0; i < numFloors; ++i)
		floors[i] = false;

	Vector2 shaftDimensions = Vector2(BuildingData::SHAFT_WIDTH, BuildingData::FLOOR_HEIGHT*numFloors);
	shaft.reset(GameManager::guiFactory->createRectangleFrame(
		shaftTop, shaftDimensions, BuildingData::SHAFT_WALL_THICKNESS, Color(.5, 0, .5, 1)));

	Vector2 carDimensions = Vector2(BuildingData::SHAFT_WIDTH - BuildingData::SHAFT_WALL_THICKNESS * 2,
		BuildingData::FLOOR_HEIGHT / 2);
	Vector2 carPos = shaftTop;
	carPos.x += (BuildingData::SHAFT_WIDTH - carDimensions.x) / 2;
	car.reset(GameManager::guiFactory->createRectangleFrame(carPos, carDimensions));

}

Elevator::~Elevator() {
	delete[] floors;
}


void Elevator::draw(SpriteBatch* batch) {

	car->draw(batch);
	shaft->draw(batch);
}

void Elevator::callElevatorTo(unsigned short destinationFloor) {

	floors[destinationFloor] = true;

	if (state == WAITING) {
		wakeUp();

	}
}

const Vector2& Elevator::getPosition() const {

	return shaftTop;
}

const int Elevator::getWidth() const {
	return shaft->getWidth();
}

void Elevator::wakeUp() {
}
