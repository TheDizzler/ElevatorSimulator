#include "Camera.h"

Camera::Camera(int vwprtWdth, int vwprtHght) {

	zoom = 1.0f;

	viewportWidth = vwprtWdth;
	viewportHeight = vwprtHght;
	viewportCenter = Vector3(viewportWidth * .5, viewportHeight * .5, 0);

	position = Vector2::Zero;
}

Camera::~Camera() {
}

void Camera::setBuilding(Building* bldng) {

	building = bldng;

	buildingWidth = BuildingData::BUILDING_LENGTH;
	buildingHeight = BuildingData::BUILDING_HEIGHT;

	viewX = (viewportWidth / zoom / 2);
	viewY = (viewportHeight / zoom / 2);

}

void Camera::adjustZoom(float amount) {

	zoom += amount;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;

	viewX = (viewportWidth / zoom / 2);
	viewY = (viewportHeight / zoom / 2);
}


void Camera::moveCamera(Vector2 cameraMovement, bool clampToBuilding) {

	Vector2 newPos = position + cameraMovement;

	if (clampToBuilding)
		buildingClampedPosition(newPos);

	position = newPos;

}


RECT* Camera::viewportWorldBoundary() {

	Vector2* viewportCorner = screenToWorld(Vector2::Zero);
	Vector2* viewportBottomCorner =
		screenToWorld(Vector2(viewportWidth, viewportHeight));

	RECT* rect = new RECT{
		(int) viewportCorner->x, (int) viewportCorner->y,
		(int) (viewportBottomCorner->x - viewportCorner->x),
		(int) (viewportBottomCorner->y - viewportCorner->y)};

	delete viewportCorner;
	delete viewportBottomCorner;

	return rect;
}


void Camera::centerOn(Vector2 pos, bool clampToBuilding) {

	if (clampToBuilding)
		buildingClampedPosition(pos);

	position = pos;
}


Vector2* Camera::screenToWorld(Vector2 screenPosition) {

	Vector2* vec = new Vector2();
	Vector2::Transform(screenPosition, translationMatrix().Invert(), *vec);

	return vec;
}



void Camera::buildingClampedPosition(Vector2& position) {


	Vector2 cameraMax = Vector2(
		buildingWidth - viewX,
		buildingHeight - viewY);

	Vector2 cameraMin = Vector2(viewX, viewY);
	if (cameraMax.x < cameraMin.x)
		position.Clamp(cameraMax, cameraMin);
	else
		position.Clamp(cameraMin, cameraMax);
}



Matrix Camera::translationMatrix() {
	// casting to int prevents filtering artifacts??
	return Matrix::CreateTranslation(-(int) position.x, -(int) position.y, 0)
		* Matrix::CreateRotationZ(rotation)
		* Matrix::CreateScale(zoom, zoom, 1)
		* Matrix::CreateTranslation(viewportCenter);
}