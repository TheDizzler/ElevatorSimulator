#include "Exit.h"

#include "../Engine/GameEngine.h"
Exit::Exit(USHORT floorNum) {

	door = move(gfxAssets->getSpriteFromAsset("Office Door"));
	floorNumber = floorNum;



}

Exit::~Exit() {
}

void Exit::setPosition(const Vector2& position) {
	door->setPosition(position);

}

const Vector2 & Exit::getPosition() {
	return door->getPosition();
}

const int Exit::getWidth() {
	return door->getWidth();
}

const int Exit::getHeight() {
	return door->getHeight();
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

}

void Exit::draw(SpriteBatch * batch) {

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
	pos.x -= rand(rng) - door->getWidth();
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
	if (newTint.w <= .01)
		isAlive = false;
}

void Counter::draw(SpriteBatch * batch) {
	label->draw(batch);
}
