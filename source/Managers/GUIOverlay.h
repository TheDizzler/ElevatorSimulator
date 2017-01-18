#include "../pch.h"
#pragma once

#include "../GameObjects/Floor.h"

class GUIOverlay {
public:
	GUIOverlay();
	~GUIOverlay();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	void updateFloorDisplay(wstring floorNumber);
	void updateNextStopDisplay(wstring floorNumber);

	void updateStopQueue(list<shared_ptr<Stop>>  list);
	void updateUpQueue(list<shared_ptr<Stop>>  list);
	void updateDownQueue(list<shared_ptr<Stop>>  list);
	/* get area not covered by GUI */
	const Vector2& getPlayArea() const;

private:
	unique_ptr<Dialog> stopQueueDialog;
	unique_ptr<Dialog> upQueueDialog;
	unique_ptr<Dialog> downQueueDialog;

	unique_ptr<Dialog> currentFloorDisplay;
	unique_ptr<Dialog> nextStopDisplay;
	

};