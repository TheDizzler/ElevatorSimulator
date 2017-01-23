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
	wss << floorNumber/* << " at (" << floorPosition.x << ", " << floorPosition.y << ")"*/;
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


	// create at least one exit

	exit = make_shared<Exit>(floorNumber);
	Vector2 exitpos = floorPosition;
	exitpos.y -= exit->getHeight() / 2;
	mt19937 rng;
	rng.seed(random_device{}());
	bernoulli_distribution dist;

	bool leftSide = dist(rng);

	int min, max;
	if (leftSide) {
		min = floorPosition.x + exit->getWidth() / 2;
		max = buttonpos.x - exit->getWidth() / 2;
	} else {
		min = elevator->getShaftPosition().x + elevator->getWidth() + exit->getWidth() / 2;
		max = floorPosition.x + BuildingData::BUILDING_LENGTH - exit->getWidth() / 2;
	}

	uniform_int_distribution<mt19937::result_type> rand(min, max);
	exitpos.x = rand(rng);

	exit->setPosition(exitpos);

}

Floor::~Floor() {

	ridersWaiting.clear();
}

shared_ptr<Exit> Floor::getExit() {
	return exit;
}


void Floor::update(double deltaTime) {

	exit->update(deltaTime);

	double moveTime;
	switch (doorState) {

		case opening:
			timeOpening += deltaTime;
			moveTime = timeOpening / timeToOpen;
			doorLeft->setPosition(Vector2::Lerp(originalPositionLeft, endPositionLeft, moveTime));
			doorRight->setPosition(Vector2::Lerp(originalPositionRight, endPositionRight, moveTime));

			if (doorLeft->getPosition().x <= endPositionLeft.x) {
				doorState = open;
				elevator->startUnloading();
				stillLoadingElevator = true;

			}
			break;

		case open:
			if (stillLoadingElevator) { // elevator in its update does unloading

				if (elevator->stillUnloading())
					break;

				timeUntilNextTransfer -= deltaTime;
				if (timeUntilNextTransfer <= 0) {

					stillLoadingElevator = false;
					timeUntilNextTransfer = elevator->TIME_BETWEEN_TRANSFER;

					 // load up riders going in proper direction
					for (int i = 0; i < elevator->MAX_RIDERS_TRANSFERRING && i < ridersWaiting.size(); ++i) {
						Rider* rider = ridersWaiting.back();
						ridersWaiting.pop_back();
						if ((elevatorDirection == NextStopDirection::Up
							&& rider->finalDestination->floorNumber > floorNumber)
							|| (elevatorDirection == NextStopDirection::Down
								&& rider->finalDestination->floorNumber < floorNumber)) {

							rider->enterElevator(elevator.get());
							stillLoadingElevator = true;
						}
					}

					if (!stillLoadingElevator) {
						timeUntilNextTransfer = 0;
						elevator->doneTransferring();
					}
				}
			} else {

				timeOpen += deltaTime;
				if (timeOpen >= timeToStayOpen && elevator->hasNextStop()) {
					doorState = closing;
					timeOpen = 0;
				}
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
						callButtons->elevatorArrivedGoingUp();
						break;
					case Elevator::ElevatorState::GoingDown:
						downIndicatorLight = downIndicatorOff.get();
						callButtons->elevatorArivedGoingDown();
						break;
				}
				elevatorOnFloor = false;
				elevatorDirection = NextStopDirection::None;
			}
			break;

		case closed:

			if (openDoors) {
				doorState = opening;
				door1 = doorLeft.get();
				door2 = doorRight.get();
				openDoors = false;
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

	exit->draw(batch);
}

int Floor::callButtonLocation() {
	return callButtons->getPosition().x;
}

bool Floor::doorsOpen() {
	return doorState != DoorState::closed;
}

void Floor::pushUpButton(Rider* rider) {


	if (!callButtons->upButtonPressed) {

		if (elevator->getCurrentFloor().get() == this) {
			doorState = DoorState::opening;
		} else {
			callButtons->pushUpButton();
			elevator->callElevatorTo(floorNumber, true);
		}
	}

	ridersWaiting.push_back(rider);
}


void Floor::pushDownButton(Rider* rider) {

	if (!callButtons->downButtonPressed) {

		if (elevator->getCurrentFloor().get() == this) {
			doorState = DoorState::opening;
		} else {
			callButtons->pushUpButton();
			elevator->callElevatorTo(floorNumber, false);
		}
	}

	ridersWaiting.push_back(rider);
}

void Floor::getInElevator(Rider* rider) {
	ridersWaiting.push_back(rider);
}

bool Floor::elevatorGoingUpDoorOpen() {
	return doorState == DoorState::open &&
		(elevatorDirection == NextStopDirection::Up || elevatorDirection == NextStopDirection::None);
}

bool Floor::elevatorGoingDownDoorOpen() {
	return doorState == DoorState::open &&
		(elevatorDirection == NextStopDirection::Down || elevatorDirection == NextStopDirection::None);
}


void Floor::elevatorArrived(bool elevatorGoingUp) {

	elevatorOnFloor = true;
	openDoors = true;
	if (elevatorGoingUp)
		callButtons->elevatorArrivedGoingUp();
	else
		callButtons->elevatorArivedGoingDown();

}

void Floor::elevatorApproaching(NextStopDirection nsd) {

	elevatorDirection = nsd;
	switch (nsd) {
		case NextStopDirection::Up:
			upIndicatorLight = upIndicatorOn.get();
			break;
		case NextStopDirection::Down:
			downIndicatorLight = downIndicatorOn.get();
			break;
		case NextStopDirection::None:
			break;
	}
}
