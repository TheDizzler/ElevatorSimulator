#include "Exit.h"

#include "../Engine/GameEngine.h"
Exit::Exit() {

	door = move(gfxAssets->getSpriteFromAsset("Office Door"));
	
}

Exit::~Exit() {
}

void Exit::setPosition(const Vector2 & position) {
	door->setPosition(position);
}

const int Exit::getWidth() {
	return door->getWidth();
}

const int Exit::getHeight() {
	return door->getHeight();
}


void Exit::update(double deltaTime) {
}

void Exit::draw(SpriteBatch * batch) {

	door->draw(batch);
}
