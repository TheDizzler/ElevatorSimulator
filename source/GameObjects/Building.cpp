#include "Building.h"

#include "../globals.h"

USHORT BuildingData::PIXELS_PER_METER = 48;
USHORT BuildingData::FLOOR_HEIGHT = 2 * PIXELS_PER_METER;
USHORT BuildingData::BUILDING_LENGTH = 25 * PIXELS_PER_METER;
USHORT BuildingData::BUILDING_WALL_THICKNESS = 6;
USHORT BuildingData::BUILDING_HEIGHT;
Vector2 BuildingData::BUILDING_POSITION = Vector2(10, 10);


USHORT BuildingData::SHAFT_WIDTH = 2 * PIXELS_PER_METER;
USHORT BuildingData::SHAFT_WALL_THICKNESS = 4;



BuildingData::BuildingData(USHORT numFloors) {

	BUILDING_HEIGHT = FLOOR_HEIGHT * numFloors;

	BUILDING_POSITION = Vector2((Globals::WINDOW_WIDTH - BUILDING_LENGTH) / 2,
		(Globals::WINDOW_HEIGHT - BUILDING_HEIGHT) / 2);
}




#include "../Engine/GameEngine.h"
Building::Building(unsigned short numFloors) {

	// build building data	
	BuildingData::BuildingData(numFloors);

	// construct building
	outline.reset(GameManager::guiFactory->createRectangleFrame(
		BuildingData::BUILDING_POSITION,
		Vector2(BuildingData::BUILDING_LENGTH, BuildingData::BUILDING_HEIGHT), BuildingData::BUILDING_WALL_THICKNESS));


	Vector2 floorPos = BuildingData::BUILDING_POSITION;
	floorPos.y += BuildingData::BUILDING_HEIGHT - BuildingData::BUILDING_WALL_THICKNESS / 2;

	for (int floorNum = 1; floorNum < numFloors; ++floorNum) {

		floorPos.y -= BuildingData::FLOOR_HEIGHT;

		unique_ptr<Floor> floor;
		/*floor.reset(GameManager::guiFactory->createLine(
			floorPos, Vector2(BuildingData::BUILDING_LENGTH, 2)));*/
		floor.reset(new Floor(floorNum, floorPos));
		floors.push_back(move(floor));
	}


	// construct elevator
	Vector2 shaftTop = BuildingData::BUILDING_POSITION;
	shaftTop.x += BuildingData::BUILDING_LENGTH / 2;

	elevator = new Elevator(shaftTop, numFloors);


	riderStart = BuildingData::BUILDING_POSITION;
	riderStart.y += BuildingData::BUILDING_HEIGHT - BuildingData::BUILDING_WALL_THICKNESS / 2;

	generateRider();
}

Building::~Building() {

	for each (Rider* rider in riders)
		delete rider;

	riders.clear();

	floors.clear();
}

void Building::update(double deltaTime) {

//update riders


// update elevator


}

void Building::draw(SpriteBatch* batch) {

	// draw building
	outline->draw(batch);

	for each (auto& floor in floors)
		floor->draw(batch);

	// draw elevator
	elevator->draw(batch);

	// draw riders
	for each (Rider* rider in riders)
		rider->draw(batch);
}


void Building::generateRider() {

	unsigned short destinationFloor = 2;

	Rider* rider = new Rider(destinationFloor);
	rider->load(GameManager::gfxAssets->getAsset("Rider"));
	rider->setPosition(riderStart);

	riders.push_back(rider);
}

