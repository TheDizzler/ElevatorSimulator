#include "../pch.h"
#pragma once

#include "CallButtons.h"


class Floor {
public:
	Floor(USHORT floorNum, Vector2 floorPosition, const Elevator* const elevator);
	~Floor();


	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	Vector2 position;

	bool elevatorArrived = false;

private:

	unique_ptr<Line> line; // this is the floor
	unique_ptr<CallButtons> callButtons;

	const Elevator* elevator;
	USHORT floorNumber;

	shared_ptr<AssetSet> elevatorAssets;

	unique_ptr<Sprite> doorFrame;
	unique_ptr<Sprite> doorsClosed;
	unique_ptr<Sprite> doorLeft;
	unique_ptr<Sprite> doorRight;



	Sprite* door1;
	Sprite* door2 = NULL;

	enum DoorState {
		closed, opening, open, closing
	};
	DoorState doorState = closed;

	double timeToOpen = 2.0;
	double timeOpening = 0.0;

	double timeToStayOpen = 3.0;
	double timeOpen = 0.0;

	Vector2 originalPositionLeft;
	Vector2 endPositionLeft;
	Vector2 originalPositionRight;
	Vector2 endPositionRight;

};