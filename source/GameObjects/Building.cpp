#include "Building.h"

#include "../globals.h"

size_t NUM_FLOORS;
size_t BuildingData::PIXELS_PER_METER = 48;
size_t BuildingData::FLOOR_HEIGHT = 2.5 * PIXELS_PER_METER;
size_t BuildingData::BUILDING_LENGTH = 10 * PIXELS_PER_METER;
size_t BuildingData::BUILDING_WALL_THICKNESS = 6;
size_t BuildingData::BUILDING_HEIGHT;
Vector2 BuildingData::BUILDING_POSITION = Vector2(10, 10);


size_t BuildingData::SHAFT_WIDTH = 2 * PIXELS_PER_METER;
size_t BuildingData::SHAFT_WALL_THICKNESS = 4;



BuildingData::BuildingData(size_t numFloors) {

	BUILDING_HEIGHT = FLOOR_HEIGHT * numFloors;

	BUILDING_POSITION = Vector2((Globals::WINDOW_WIDTH - BUILDING_LENGTH) / 2,
		(Globals::WINDOW_HEIGHT - BUILDING_HEIGHT) / 2);
}


#include "../Engine/GameEngine.h"
Building::Building(size_t numFloors) {

	NUM_FLOORS = numFloors;
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

		shared_ptr<Floor> floor = make_shared<Floor>(this, floorNum, floorPos, elevator);
		floors.push_back(move(floor));

		floorPos.y -= BuildingData::FLOOR_HEIGHT;
	}

	elevator->setFloors(floors);

	//generateRider(1);

	buildingEntrance = make_unique<Exit>(this, 1);
	buildingEntrance->setPosition(floors[0]->position);
	buildingEntrance->moveBy(Vector2(0, -buildingEntrance->getHeight()));
	guiOverlay->newRiderButton->setOnClickListener(getNewRiderButton(buildingEntrance.get()));
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



NewRiderButtonListener* Building::getNewRiderButton(Exit* exit) {
	return new NewRiderButtonListener(this, exit);
}


void Building::generateRider(Exit*  exit) {

	int min = 1;
	int max = floors.size();
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand(min, max);
	unsigned short destinationFloorNum = rand(rng);

	shared_ptr<Rider> rider = make_shared<Rider>(gfxAssets->getAsset("Rider"),
		floors[exit->floorNumber - 1], exit, floors[destinationFloorNum - 1]->getExit());

	rider->moveBy(Vector2(exit->getWidth(), exit->getHeight()));
	riders.push_back(rider);
}

