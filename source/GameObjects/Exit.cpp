#include "Exit.h"

#include "../Engine/GameEngine.h"
#include "Building.h"
Exit::Exit(USHORT floorNum) {

	floorNumber = floorNum;
	door.reset((ImageButton*) guiFactory->createImageButton(move(gfxAssets->getSpriteFromAsset("Office Door"))));
	door->setOnClickListener(building->getNewRiderButton(this));
	door->addCamera(camera);

}

Exit::~Exit() {
}

void Exit::setPosition(const Vector2& pos) {

	Vector2 position = pos;
	door->setPosition(position);

}

void Exit::moveBy(const Vector2& moveAmount) {
	door->moveBy(moveAmount);
}

const Vector2& Exit::getPosition() {
	return door->getPosition();
}

const int Exit::getWidth() {
	return door->getScaledWidth();
}

const int Exit::getHeight() {
	return door->getScaledHeight();
}

const HitArea* Exit::getHitArea() {
	return door->getHitArea();
}


void Exit::update(double deltaTime) {

	for (auto& const counter : counters)
		counter->update(deltaTime);

	counters.erase(remove_if(counters.begin(), counters.end(), [](auto& const counter) {
		return !counter->isAlive;
	}), counters.end());

	door->update(deltaTime);

}

void Exit::draw(SpriteBatch* batch) {

	door->draw(batch);
	for (auto& const counter : counters)
		counter->draw(batch);

}

#include "Rider.h"
void Exit::riderArrived(Rider* rider) {

	++numRidersExited;
	Vector2 pos = door->getPosition();
	//pos.x -= door->getWidth() / 2;
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand(0, door->getWidth() * 2);
	int rndNum = rand(rng) - door->getWidth();
	pos.x -= rndNum;
	unique_ptr<Counter> counter = make_unique<Counter>(pos);
	counters.push_back(move(counter));

}



int Counter::counterSpeed = 25;
Color Counter::originalColor = Color(DirectX::Colors::Blue);
Color Counter::endColor;
Counter::Counter(const Vector2& counterPosition) {

	label.reset(guiFactory->createTextLabel(counterPosition, L"+1"));
	label->setTint(originalColor);
	label->setScale(Vector2(1.5, 1.5));
	endColor = originalColor;
	/*endColor.x = 1;
	endColor.y = 1;
	endColor.z = 1;*/
	endColor.w = 0;
}

Counter::~Counter() {
}

void Counter::update(double deltaTime) {

	timeAlive += deltaTime;
	label->moveBy(Vector2(0, -counterSpeed * deltaTime));
	Color newTint = Color::Lerp(originalColor, endColor, timeAlive / 3);
	label->setTint(newTint);
	if (/*newTint.w <= .01*/ timeAlive >= 3)
		isAlive = false;
}

void Counter::draw(SpriteBatch* batch) {
	label->draw(batch);
}
