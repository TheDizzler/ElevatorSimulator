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
	Building();
	~Building();

	void initBuilding();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	NewRiderButtonListener* getNewRiderButton(USHORT floorNumber);

	void generateRider(USHORT startFloorNumber);
private:

	list<shared_ptr<Rider>> riders;
	shared_ptr<Elevator> elevator;

	unique_ptr<RectangleFrame> outline;
	vector<shared_ptr<Floor> > floors;



};

class NewRiderButtonListener : public Button::OnClickListener {
public:
	NewRiderButtonListener(Building* build, USHORT floorNum) : building(build), floorNumber(floorNum) {
	}
	virtual void onClick(Button* button) override {
		building->generateRider(floorNumber);
	}

	Building* building;

private:
	USHORT floorNumber;
};