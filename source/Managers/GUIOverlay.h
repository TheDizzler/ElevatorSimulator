#include "../pch.h"
#pragma once

#include "../GameObjects/Floor.h"

class GUIOverlay {
public:
	GUIOverlay();
	~GUIOverlay();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);


	void updateQueueDisplay(list<shared_ptr<Floor> > list);
	/* get area not covered by GUI */
	const Vector2& getPlayArea() const;

private:
	unique_ptr<Dialog> elevatorQueueDialog;

};