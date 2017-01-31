#include "../pch.h"
#pragma once

class Rider;
class Building;

class Counter {
public:
	Counter(const Vector2& counterPosition);
	~Counter();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	bool isAlive = true;
private:

	unique_ptr<TextLabel> label;

	double timeAlive = 0;

	static int counterSpeed;
	static Color originalColor;
	static Color endColor;
};

class Exit {
public:
	Exit(Building* building, size_t floorNumber);
	~Exit();

	void setPosition(const Vector2& position);
	void moveBy(const Vector2& moveAmount);

	const Vector2& getPosition();
	const int getWidth();
	const int getHeight();

	const HitArea* getHitArea();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	size_t floorNumber;

	void riderArrived(Rider* rider);

private:

	unique_ptr<ImageButton> door;

	vector<unique_ptr<Counter>> counters;

	size_t numRidersExited = 0;
	size_t numRuidersGenerated = 0;
};