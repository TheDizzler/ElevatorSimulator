#pragma once

#include "../DXTKGui/Controls/TextLabel.h"

/** A bubble showing a Rider's current thought. */
class RiderBubble {
public:
	RiderBubble();
	~RiderBubble();

	void setThought(wstring thought);

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	void setPosition(const Vector2& newPosition);
	void moveBy(const Vector2& moveAmount);

	const float getHeight() const;
private:
	unique_ptr<Sprite> sprite;
	unique_ptr<TextLabel> label;

	//static Vector2 bubbleOffset;
	//static Vector2 labelOffset;

	const double BUBBLE_FLASH_TIME = 1.5;
	double timeOn = 0;
	bool alphaIncreasing = true;

};