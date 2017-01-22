#include "../pch.h"
#pragma once

//#include "../DXTKGui/BaseGraphics/PrimitiveShapes.h"

#include "Elevator.h"
#include "Rider.h"
#include "Floor.h"



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


	void update(double deltaTime);
	void draw(SpriteBatch* batch);


	void generateRider();
private:

	list<shared_ptr<Rider>> riders;
	shared_ptr<Elevator> elevator;

	unique_ptr<RectangleFrame> outline;
	vector<shared_ptr<Floor> > floors;

	


	//Vector2 riderStart;
	shared_ptr<Floor> riderStartFloor;

};

class NewRiderButtonListener : public Button::OnClickListener {
public:
	NewRiderButtonListener(Building* build) : building(build) {
	}
	virtual void onClick(Button * button) override {
		building->generateRider();
	}

	Building* building;
};