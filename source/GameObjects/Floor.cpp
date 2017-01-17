#include "Floor.h"

//#include "../Managers/GameManager.h"
#include "../Engine/GameEngine.h"
Floor::Floor(USHORT floorNum, Vector2 floorPosition, shared_ptr<Elevator> elev) {

	line.reset(guiFactory->createLine(
		floorPosition, Vector2(BuildingData::BUILDING_LENGTH, 2)));

	elevator = elev;
	floorNumber = floorNum;

	elevatorAssets = gfxAssets->getAssetSet("Elevator Door");


	doorFrame = make_unique<Sprite>();
	doorFrame->load(elevatorAssets->getAsset("Elevator Door Frame"));
	Vector2 doorpos(elevator->getShaftPosition().x + elevator->getWidth() / 2,
		floorPosition.y - doorFrame->getHeight() / 2 + 2);
	doorFrame->setPosition(doorpos);


	doorsClosed = make_unique<Sprite>();
	doorsClosed->load(elevatorAssets->getAsset("Elevator Door Closed"));
	doorsClosed->setPosition(doorpos);

	doorLeft = make_unique<Sprite>();
	doorLeft->load(elevatorAssets->getAsset("Elevator Door Left"));
	doorpos.x -= doorLeft->getWidth() / 2;
	doorLeft->setPosition(doorpos);
	originalPositionLeft = doorpos;
	endPositionLeft = originalPositionLeft;
	endPositionLeft.x -= doorLeft->getWidth() - 6;

	doorRight = make_unique<Sprite>();
	doorRight->load(elevatorAssets->getAsset("Elevator Door Right"));
	doorpos.x += doorRight->getWidth();
	doorRight->setPosition(doorpos);
	originalPositionRight = doorpos;
	endPositionRight = originalPositionRight;
	endPositionRight.x += doorRight->getWidth() - 6;


	door1 = doorsClosed.get();


	callButtons.reset(new CallButtons());

	Vector2 buttonpos = floorPosition;
	buttonpos.x = elevator->getShaftPosition().x - 25;
	buttonpos.y -= 32;
	callButtons->setPosition(buttonpos);
	position = floorPosition;

	Vector2 labelPos = Vector2(floorPosition.x + 32, floorPosition.y - BuildingData::FLOOR_HEIGHT / 2);
	floorLabel.reset(guiFactory->createTextLabel(labelPos));
	wostringstream wss;
	wss << floorNumber << " at (" << floorPosition.x << ", " << floorPosition.y << ")";
	floorLabel->setText(wss);
	floorLabel->setTint(Color(0, 0, 0));
	floorLabel->moveBy(Vector2(0, -floorLabel->getHeight() / 2));

}

Floor::~Floor() {
}


void Floor::update(double deltaTime) {

	double moveTime;
	switch (doorState) {

		case opening:
			timeOpening += deltaTime;
			moveTime = timeOpening / timeToOpen;
			doorLeft->setPosition(Vector2::Lerp(originalPositionLeft, endPositionLeft, moveTime));
			doorRight->setPosition(Vector2::Lerp(originalPositionRight, endPositionRight, moveTime));

			if (doorLeft->getPosition().x <= endPositionLeft.x) {
				doorState = open;
				timeOpen = 0;
			}
			break;

		case open:
			timeOpen += deltaTime;
			if (timeOpen >= timeToStayOpen) {
				doorState = closing;
				timeOpen = 0;
			}
			break;


		case closing:
			timeOpening -= deltaTime;
			moveTime = timeOpening / timeToOpen;
			doorLeft->setPosition(Vector2::Lerp(originalPositionLeft, endPositionLeft, moveTime));
			doorRight->setPosition(Vector2::Lerp(originalPositionRight, endPositionRight, moveTime));

			if (doorLeft->getPosition().x >= originalPositionLeft.x) {
				doorState = closed;
				door1 = doorsClosed.get();
				door2 = NULL;
			}
			break;

		case closed:

			if (elevatorOnFloor) {
				doorState = opening;
				door1 = doorLeft.get();
				door2 = doorRight.get();
				elevatorOnFloor = false;
			}
			break;
	}
}

void Floor::draw(SpriteBatch* batch) {

	line->draw(batch);
	door1->draw(batch);
	if (door2)
		door2->draw(batch);
	doorFrame->draw(batch);
	callButtons->draw(batch);
	floorLabel->draw(batch);

}

int Floor::callButtonLocation() {
	return callButtons->getPosition().x;
}

void Floor::pushUpButton() {

	callButtons->pushUpButton();
	elevator->callElevatorTo(floorNumber, true);
}

void Floor::elevatorArrived() {

	elevatorOnFloor = true;

}
