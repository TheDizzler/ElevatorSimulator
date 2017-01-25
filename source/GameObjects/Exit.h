#include "../pch.h"
#pragma once

class Rider;

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
	Exit(USHORT floorNumber);
	~Exit();

	void setPosition(const Vector2& position);

	const Vector2& getPosition();
	const int getWidth();
	const int getHeight();

	const HitArea* getHitArea();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	USHORT floorNumber;

	void riderArrived(Rider* rider);


private:

	//unique_ptr<Sprite> door;
	unique_ptr<ImageButton> door;

	vector<unique_ptr<Counter>> counters;

	UINT numRidersExited = 0;
	UINT numRuidersGenerated = 0;
};