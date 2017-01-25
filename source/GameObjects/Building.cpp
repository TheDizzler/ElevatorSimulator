#include "Building.h"

#include "../globals.h"

USHORT NUM_FLOORS = 3;
USHORT BuildingData::PIXELS_PER_METER = 48;
USHORT BuildingData::FLOOR_HEIGHT = 2.5 * PIXELS_PER_METER;
USHORT BuildingData::BUILDING_LENGTH = 10 * PIXELS_PER_METER;
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
Building::Building() {

	// build building data	
	/*BuildingData buildingData = */BuildingData::BuildingData(NUM_FLOORS);

	
}

Building::~Building() {

	riders.clear();
	floors.clear();
}

void Building::initBuilding() {
	// construct building
	outline.reset(guiFactory->createRectangleFrame(
		BuildingData::BUILDING_POSITION,
		Vector2(BuildingData::BUILDING_LENGTH, BuildingData::BUILDING_HEIGHT), BuildingData::BUILDING_WALL_THICKNESS));


	// construct elevator
	Vector2 shaftTop = BuildingData::BUILDING_POSITION;
	shaftTop.x += BuildingData::BUILDING_LENGTH / 2;

	elevator = make_shared<Elevator>(shaftTop, NUM_FLOORS);


	Vector2 floorPos = BuildingData::BUILDING_POSITION;
	floorPos.y += BuildingData::BUILDING_HEIGHT - BuildingData::BUILDING_WALL_THICKNESS / 2;

	for (int floorNum = 1; floorNum <= NUM_FLOORS; ++floorNum) {

		shared_ptr<Floor> floor = make_shared<Floor>(floorNum, floorPos, elevator);
		floors.push_back(move(floor));

		floorPos.y -= BuildingData::FLOOR_HEIGHT;
	}

	elevator->setFloors(floors);

	generateRider(1);

	guiOverlay->newRiderButton->setOnClickListener(getNewRiderButton(1));
}

void Building::update(double deltaTime) {

	// update floors
	for each (auto& floor in floors)
		floor->update(deltaTime);

	//update riders
	list<shared_ptr<Rider> > exitedRiders;
	for each (shared_ptr<Rider> rider in riders) {
		rider->update(deltaTime);
		if (rider->riderState == Rider::RiderState::Exited)
			exitedRiders.push_back(rider);
	}

	for (shared_ptr<Rider> rider : exitedRiders)
		riders.remove(rider);

	// update elevator
	elevator->update(deltaTime);

}

void Building::draw(SpriteBatch* batch) {



	// draw elevator
	elevator->draw(batch);

	for each (auto& floor in floors)
		floor->draw(batch);



	// draw riders
	for each (shared_ptr<Rider> rider in riders)
		rider->draw(batch);


	// draw building
	outline->draw(batch);
}

NewRiderButtonListener* Building::getNewRiderButton(USHORT floorNumber) {
	return new NewRiderButtonListener(this, floorNumber);
}



void Building::generateRider(USHORT startFloorNumber) {

	int min = 1;
	int max = floors.size();
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand(min, max);
	unsigned short destinationFloorNum = rand(rng);

	shared_ptr<Rider> rider = make_shared<Rider>(gfxAssets->getAsset("Rider"),
		floors[startFloorNumber - 1], floors[destinationFloorNum - 1]->getExit());


	riders.push_back(rider);
}

