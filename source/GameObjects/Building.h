#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/PrimitiveShapes.h"

#include "Elevator.h"
#include "Rider.h"
#include "Floor.h"


class NewRiderButtonListener;

/**At default zoom 1 meter will be assumed to be 48 pixels. */
struct BuildingData {

	BuildingData(size_t numFloors);

	static Vector2 BUILDING_POSITION;

	static size_t BUILDING_HEIGHT;

	static size_t BUILDING_WALL_THICKNESS;
	static size_t FLOOR_HEIGHT;
	static size_t BUILDING_LENGTH;

	static size_t SHAFT_WIDTH;
	static size_t SHAFT_WALL_THICKNESS;

	static size_t PIXELS_PER_METER;

};

/** A Simulate Building populated with elevators and riders. */
class Building {
public:
	Building(size_t numFloors);
	~Building();

	void initBuilding();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	//NewRiderButtonListener* getNewRiderButton(USHORT floorNumber);
	NewRiderButtonListener* getNewRiderButton(Exit* exit);

	//void generateRider(USHORT startFloorNumber);
	void generateRider(Exit* exit);
private:

	list<shared_ptr<Rider>> riders;
	shared_ptr<Elevator> elevator;

	unique_ptr<RectangleFrame> outline;
	vector<shared_ptr<Floor> > floors;

	unique_ptr<Exit> buildingEntrance;

};

class NewRiderButtonListener : public Button::OnClickListener {
public:
	NewRiderButtonListener(Building* build, Exit* xt) : building(build), exit(xt) {
	}

	virtual void onClick(Button* button) override {
		building->generateRider(exit);
	}

private:
	Building* building;
	Exit* exit;
};