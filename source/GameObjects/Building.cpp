#include "Building.h"

#include "../globals.h"

USHORT BuildingData::PIXELS_PER_METER = 48;
USHORT BuildingData::FLOOR_HEIGHT = 2.5 * PIXELS_PER_METER;
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
	outline.reset(guiFactory->createRectangleFrame(
		BuildingData::BUILDING_POSITION,
		Vector2(BuildingData::BUILDING_LENGTH, BuildingData::BUILDING_HEIGHT), BuildingData::BUILDING_WALL_THICKNESS));


	// construct elevator
	Vector2 shaftTop = BuildingData::BUILDING_POSITION;
	shaftTop.x += BuildingData::BUILDING_LENGTH / 2;

	elevator = make_shared<Elevator>(shaftTop, numFloors);


	Vector2 floorPos = BuildingData::BUILDING_POSITION;
	floorPos.y += BuildingData::BUILDING_HEIGHT - BuildingData::BUILDING_WALL_THICKNESS / 2;

	for (int floorNum = 1; floorNum <= numFloors; ++floorNum) {

		shared_ptr<Floor> floor = make_shared<Floor>(floorNum, floorPos, elevator);
		floors.push_back(move(floor));

		floorPos.y -= BuildingData::FLOOR_HEIGHT;
	}

	elevator->setFloors(floors);


	riderStartFloor = floors[0];

	generateRider();
}

Building::~Building() {

	for each (Rider* rider in riders)
		delete rider;

	riders.clear();

	floors.clear();
}

void Building::update(double deltaTime) {

	// update floors
	for each (auto& floor in floors)
		floor->update(deltaTime);
	//update riders
	for each (Rider* rider in riders)
		rider->update(deltaTime);

// update elevator
	elevator->update(deltaTime);

}

void Building::draw(SpriteBatch* batch) {



	// draw elevator
	elevator->draw(batch);

	for each (auto& floor in floors)
		floor->draw(batch);



	// draw riders
	for each (Rider* rider in riders)
		rider->draw(batch);


	// draw building
	outline->draw(batch);
}


void Building::generateRider() {

	unsigned short destinationFloor = 2;

	Rider* rider = new Rider(gfxAssets->getAsset("Rider"), riderStartFloor, destinationFloor);
	//rider->setSprite(gfxAssets->getAsset("Rider"));
	//rider->setFloor(riderStartFloor);

	riders.push_back(rider);
}

