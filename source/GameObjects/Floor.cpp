#include "Floor.h"

//#include "../Managers/GameManager.h"
#include "../Engine/GameEngine.h"
Floor::Floor(USHORT floorNum, Vector2 floorPosition, shared_ptr<Elevator> elev) {

	line.reset(guiFactory->createLine(
		floorPosition, Vector2(BuildingData::BUILDING_LENGTH, 2)));

	elevator = elev;
	floorNumber = floorNum;

	elevatorDoorAssets = gfxAssets->getAssetSet("Elevator Door");


	doorFrame = make_unique<Sprite>();
	doorFrame->load(elevatorDoorAssets->getAsset("Elevator Door Frame"));
	Vector2 doorpos(elevator->getShaftPosition().x + elevator->getWidth() / 2,
		floorPosition.y - doorFrame->getHeight() / 2 + 2);
	doorFrame->setPosition(doorpos);


	doorsClosed = make_unique<Sprite>();
	doorsClosed->load(elevatorDoorAssets->getAsset("Elevator Door Closed"));
	doorsClosed->setPosition(doorpos);

	doorLeft = make_unique<Sprite>();
	doorLeft->load(elevatorDoorAssets->getAsset("Elevator Door Left"));
	doorpos.x -= doorLeft->getWidth() / 2;
	doorLeft->setPosition(doorpos);
	originalPositionLeft = doorpos;
	endPositionLeft = originalPositionLeft;
	endPositionLeft.x -= doorLeft->getWidth() - 6;

	doorRight = make_unique<Sprite>();
	doorRight->load(elevatorDoorAssets->getAsset("Elevator Door Right"));
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


	indicatorAssets = gfxAssets->getAssetSet("Direction Indicators");


	indicatorScale = Vector2(.9, .9);


	upIndicatorOn = make_unique<Sprite>();
	upIndicatorOn->load(indicatorAssets->getAsset("Direction Indicator Up On"));
	upIndicatorOn->setScale(indicatorScale);

	Vector2 upIndicatorPos = Vector2(elev->getShaftPosition().x, floorPosition.y);
	upIndicatorPos.x += (elev->getWidth() - upIndicatorOn->getWidth()) / 2;
	upIndicatorPos.y -= BuildingData::FLOOR_HEIGHT - upIndicatorOn->getHeight();
	Vector2 downIndicatorPos = upIndicatorPos;
	downIndicatorPos.x = elev->getShaftPosition().x + elev->getWidth()
		- (elev->getWidth() - upIndicatorOn->getWidth()) / 2;

	upIndicatorOn->setPosition(upIndicatorPos);


	upIndicatorOff = make_unique<Sprite>();
	upIndicatorOff->load(indicatorAssets->getAsset("Direction Indicator Up Off"));
	upIndicatorOff->setPosition(upIndicatorPos);
	upIndicatorOff->setScale(indicatorScale);

	downIndicatorOn = make_unique<Sprite>();
	downIndicatorOn->load(indicatorAssets->getAsset("Direction Indicator Down On"));
	downIndicatorOn->setPosition(downIndicatorPos);
	downIndicatorOn->setScale(indicatorScale);

	downIndicatorOff = make_unique<Sprite>();
	downIndicatorOff->load(indicatorAssets->getAsset("Direction Indicator Down Off"));
	downIndicatorOff->setPosition(downIndicatorPos);
	downIndicatorOff->setScale(indicatorScale);


	upIndicatorLight = upIndicatorOff.get();
	downIndicatorLight = downIndicatorOff.get();

}

Floor::~Floor() {

	ridersWaiting.clear();
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
				for (Rider* rider : ridersWaiting)
					rider->enterElevator(elevator.get());
			}
			break;

		case open:
			timeOpen += deltaTime;
			if (timeOpen >= timeToStayOpen && elevator->hasNextStop()) {
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
				elevator->doorsClosed();
				switch (elevator->state) {
					case Elevator::ElevatorState::GoingUp:
						upIndicatorLight = upIndicatorOff.get();
						break;
					case Elevator::ElevatorState::GoingDown:
						downIndicatorLight = downIndicatorOff.get();
						break;
				}
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

	upIndicatorLight->draw(batch);
	downIndicatorLight->draw(batch);
}

int Floor::callButtonLocation() {
	return callButtons->getPosition().x;
}

void Floor::pushUpButton(Rider* rider) {

	callButtons->pushUpButton();
	elevator->callElevatorTo(floorNumber, true);
	ridersWaiting.push_back(rider);
}

void Floor::elevatorArrived() {

	elevatorOnFloor = true;

}

void Floor::elevatorApproaching(bool riderGoingUp) {

	if (riderGoingUp) {
		upIndicatorLight = upIndicatorOn.get();

	} else
		downIndicatorLight = downIndicatorOn.get();
}
