#include "Rider.h"

Rider::Rider(unsigned short destination) {

	destinationFloor = destination;

	sprite.reset(new Sprite());
}

Rider::~Rider() {
}

void Rider::setSprite(GraphicsAsset* gfxAsset) {

	sprite->load(gfxAsset);
}

void Rider::setFloor(Floor* floor) {

	currentFloor = floor;
	Vector2 pos = floor->position;
	pos.x -= sprite->getWidth() / 2;
	pos.y -= sprite->getHeight() / 2;
	sprite->setPosition(pos);

}

void Rider::draw(SpriteBatch* batch) {
	sprite->draw(batch);
}
