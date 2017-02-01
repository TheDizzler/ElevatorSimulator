#include "Rider Bubble.h"

//Vector2 RiderBubble::bubbleOffset = Vector2(32, 10);
//Vector2 RiderBubble::labelOffset = Vector2(

#include "../Engine/GameEngine.h"
RiderBubble::RiderBubble() {

	sprite = move(gfxAssets->getSpriteFromAsset("Thought Bubble"));
	
}

RiderBubble::~RiderBubble() {
}

void RiderBubble::setThought(wstring thought) {
	label->setText(thought);
	Vector2 labelsize = label->measureString();
	Vector2 labelpos = sprite->getPosition();
	labelpos.x -= (sprite->getWidth() / 2 - labelsize.x*2/3) /*/ 2*/;
	labelpos.y -= (sprite->getHeight() *2/ 3 - labelsize.y/2)/* / 2*/;
	label->setPosition(labelpos);
}

float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

void RiderBubble::update(double deltaTime) {

	if (alphaIncreasing)
		timeOn += deltaTime;
	else
		timeOn -= deltaTime;

	float alpha = lerp(-1, 1, timeOn / BUBBLE_FLASH_TIME);
	sprite->setAlpha(alpha);
	label->setAlpha(alpha);

	if (alpha >= 1)
		alphaIncreasing = false;
	else if (alpha <= 0)
		alphaIncreasing = true;

}


void RiderBubble::draw(SpriteBatch* batch) {

	sprite->draw(batch);
	label->draw(batch);
}

void RiderBubble::setPosition(const Vector2& newPosition) {

	sprite->setPosition(newPosition);
	Vector2 labelpos = sprite->getPosition();
	labelpos.x += sprite->getWidth() / 2;
	labelpos.y += sprite->getHeight() / 2;
	label.reset(guiFactory->createTextLabel(labelpos));
	label->setTint(Vector4(0, 0, 0, 1));
}

void RiderBubble::moveBy(const Vector2& moveAmount) {
	sprite->moveBy(moveAmount);
	label->moveBy(moveAmount);
}

const float RiderBubble::getHeight() const {
	return sprite->getHeight();
}
