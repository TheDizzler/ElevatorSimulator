#include "../pch.h"
#pragma once

#include "../GameObjects/Building.h"


/** Based on
https://roguesharp.wordpress.com/2014/07/13/tutorial-5-creating-a-2d-camera-with-pan-and-zoom-in-monogame/ */
class Camera {
public:
	Camera(int viewportWidth, int viewportHeight);
	Camera(const Vector2& viewport);
	~Camera();

	void updateViewport(const Vector2& viewportArea, const Vector2& viewportPosition, bool zoomToFitBuilding);


	Viewport* viewport;
	

	float rotation = 0.0f;

	Vector2 viewportPosition;
	int viewportWidth;
	int viewportHeight;
	Vector3 viewportCenter;


	void setBuilding(Building* building);
	void adjustZoom(float amount);
	/** Move the camera in an X and Y amount based on the cameraMovement param.
	*	If clampToMap is true the camera will try not to pan outside of the
	*	bounds of the map. */
	void moveCamera(Vector2 cameraMovement, bool showWholeBuilding = true);

	RECT* viewportWorldBoundary();

	void centerOn(Vector2 pos, bool clampToBuilding = true);

	Matrix translationMatrix();
	Vector2& worldToScreen(Vector2 worldPosition);
private:

	Vector2 position;

	float zoom;
	float buildingWidth;
	float buildingHeight;

	float viewX = (viewportWidth / zoom / 2);
	float viewY = (viewportHeight / zoom / 2);

	Building* building = NULL;

	void zoomToFitBuilding();
	void buildingClampedPosition(Vector2& position);
	Vector2* screenToWorld(Vector2 screenPosition);
	
};