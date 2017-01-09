#include "Floor.h"

#include "../Managers/GameManager.h"
Floor::Floor(USHORT floorNum, Vector2 floorPosition) {

	line.reset(GameManager::guiFactory->createLine(
		floorPosition, Vector2(BuildingData::BUILDING_LENGTH, 2)));

	callButtons.reset(new CallButtons());
}

Floor::~Floor() {
}


void Floor::draw(SpriteBatch* batch) {

	line->draw(batch);

}
