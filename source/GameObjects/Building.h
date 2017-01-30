#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/PrimitiveShapes.h"

#include "Elevator.h"
#include "Rider.h"
#include "Floor.h"


class NewRiderButtonListener;

/**At default zoom 1 meter will be assumed to be 48 pixels. */
struct BuildingData {

	BuildingData(USHORT numFloors);

	static Vector2 BUILDING_POSITION;

	static USHORT BUILDING_HEIGHT;

	static USHORT BUILDING_WALL_THICKNESS;
	static USHORT FLOOR_HEIGHT;
	static USHORT BUILDING_LENGTH;

	static USHORT SHAFT_WIDTH;
	static USHORT SHAFT_WALL_THICKNESS;

	static USHORT PIXELS_PER_METER;

};

/** A Simulate Building populated with elevators and riders. */
class Building {
public:
	Building(USHORT numFloors);
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
	/*NewRiderButtonListener(Building* build, USHORT floorNum) : building(build), floorNumber(floorNum) {
	}*/
	NewRiderButtonListener(Building* build, Exit* xt) : building(build), exit(xt) {

	}


	virtual void onClick(Button* button) override {
		//building->generateRider(floorNumber);
		building->generateRider(exit);
	}


private:
	//USHORT floorNumber;
	Building* building;
	Exit* exit;
};