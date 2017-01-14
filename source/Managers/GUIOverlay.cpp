#include "GUIOverlay.h"


#include "GameManager.h"
#include "../Globals.h"
GUIOverlay::GUIOverlay() {

	elevatorQueueDialog.reset(GameManager::guiFactory->createDialog(true));
	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2);
	dialogPos = Vector2(Globals::WINDOW_WIDTH - dialogSize.x, 0);
	elevatorQueueDialog->setDimensions(dialogPos, dialogSize);
	elevatorQueueDialog->setTint(Color(1, 0, 0));
	
	
	unique_ptr<Button> quitButton2;
	quitButton2.reset(GameManager::guiFactory->createButton());
	quitButton2->setText(L"Confirm");
	elevatorQueueDialog->setConfirmButton(move(quitButton2));
	elevatorQueueDialog->setCancelButton(L"Cancel");

	elevatorQueueDialog->setTitle(L"Test");
	elevatorQueueDialog->setText(L"Temp Messsage");
	

	elevatorQueueDialog->open();
}

GUIOverlay::~GUIOverlay() {
}


void GUIOverlay::update(double deltaTime) {

	elevatorQueueDialog->update(deltaTime);
}

void GUIOverlay::draw(SpriteBatch* batch) {

	elevatorQueueDialog->draw(batch);
}

void GUIOverlay::updateQueueDisplay(list<shared_ptr<Floor>> stopQueue) {

	wostringstream wss;
	wss << "StopQueue \n";
	for each (shared_ptr<Floor> floor in stopQueue)
		wss << floor->floorNumber << "\n";
	elevatorQueueDialog->setText(wss.str());
}

const Vector2& GUIOverlay::getPlayArea() const {

	Vector2 viewArea(Globals::WINDOW_WIDTH - elevatorQueueDialog->getWidth(),
		Globals::WINDOW_HEIGHT /*- elevatorQueueDialog->getHeight()*/);
	return viewArea;
}
