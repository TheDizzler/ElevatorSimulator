#include "GUIOverlay.h"


#include "../Engine/GameEngine.h"
#include "../Globals.h"
GUIOverlay::GUIOverlay() {

	downQueueDialog.reset(guiFactory->createDialog(false));
	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2(Globals::WINDOW_WIDTH / 6, Globals::WINDOW_HEIGHT / 3);
	dialogPos = Vector2(Globals::WINDOW_WIDTH - dialogSize.x, 0);
	downQueueDialog->setDimensions(dialogPos, dialogSize);
	downQueueDialog->setTint(Color(1, 0, 0));
	downQueueDialog->setTitle(L"Down Queue");

	dialogPos.x = Globals::WINDOW_WIDTH - downQueueDialog->getWidth();
	dialogSize.x = downQueueDialog->getWidth();
	downQueueDialog->setDimensions(dialogPos, dialogSize);

	upQueueDialog.reset(guiFactory->createDialog(false));
	dialogPos.y += dialogSize.y;
	upQueueDialog->setDimensions(dialogPos, dialogSize);
	upQueueDialog->setTitle(L"Up Queue");

	stopQueueDialog.reset(guiFactory->createDialog(false));
	dialogPos.y += dialogSize.y;
	stopQueueDialog->setDimensions(dialogPos, dialogSize);
	stopQueueDialog->setTitle(L"Stop Queue");


	currentFloorDisplay.reset(guiFactory->createDialog(false, true));
	dialogPos = Vector2((Globals::WINDOW_WIDTH - dialogSize.x) / 2, 10);
	dialogSize.y = 96;
	currentFloorDisplay->setDimensions(dialogPos, dialogSize);
	currentFloorDisplay->setTitle(L"Current Floor:");

	nextStopDisplay.reset(guiFactory->createDialog(false, true));
	dialogPos.y = Globals::WINDOW_HEIGHT - dialogPos.y - dialogSize.y;
	nextStopDisplay->setDimensions(dialogPos, dialogSize);
	nextStopDisplay->setTitle(L"Next Stop:");

	stopQueueDialog->open();
	upQueueDialog->open();
	downQueueDialog->open();
	currentFloorDisplay->open();
	nextStopDisplay->open();

}

GUIOverlay::~GUIOverlay() {
}


void GUIOverlay::update(double deltaTime) {

	stopQueueDialog->update(deltaTime);
	upQueueDialog->update(deltaTime);
	downQueueDialog->update(deltaTime);
	currentFloorDisplay->update(deltaTime);
	nextStopDisplay->update(deltaTime);
}

void GUIOverlay::draw(SpriteBatch* batch) {

	stopQueueDialog->draw(batch);
	upQueueDialog->draw(batch);
	downQueueDialog->draw(batch);
	currentFloorDisplay->draw(batch);
	nextStopDisplay->draw(batch);

}

void GUIOverlay::updateFloorDisplay(wstring floorNumber) {

	currentFloorDisplay->setText(floorNumber);
}

void GUIOverlay::updateNextStopDisplay(wstring floorNumber) {
	nextStopDisplay->setText(floorNumber);
}

#include "../GameObjects/Elevator.h"
void GUIOverlay::updateStopQueue(list<shared_ptr<Stop>> stopQueue) {

	wostringstream wss;
	for each (auto& stop in stopQueue) {
		wss << stop->floor->floorNumber;
		wss << " (";
		if (stop->dropOff) {
			wss << "DropOff";
			if (stop->pickUp)
				wss << "&";
		}
		if (stop->pickUp)
			wss << "Pickup";
		wss << ")" << "\n";
	}
	stopQueueDialog->setText(wss.str());
}

void GUIOverlay::updateUpQueue(list<shared_ptr<Stop>>  upQueue) {
	wostringstream wss;
	for each (auto& stop in upQueue)
		wss << stop->floor->floorNumber << "\n";
	upQueueDialog->setText(wss.str());
}

void GUIOverlay::updateDownQueue(list<shared_ptr<Stop>>  downQueue) {
	wostringstream wss;
	for each (auto& stop in downQueue)
		wss << stop->floor->floorNumber << "\n";
	downQueueDialog->setText(wss.str());
}

const Vector2& GUIOverlay::getPlayArea() const {

	Vector2 viewArea(Globals::WINDOW_WIDTH - stopQueueDialog->getWidth(),
		Globals::WINDOW_HEIGHT /*- stopQueueDialog->getHeight()*/);
	return viewArea;
}
