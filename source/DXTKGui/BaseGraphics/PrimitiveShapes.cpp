#include "PrimitiveShapes.h"

RectangleSprite::RectangleSprite(GraphicsAsset* const graphicsAsset)
	: Sprite() {

	Sprite::load(graphicsAsset);
	origin = Vector2(0, 0);
}

RectangleSprite::RectangleSprite(ComPtr<ID3D11ShaderResourceView> pixel,
	const Vector2& pos, const Vector2 & size) : Sprite(pos) {

	texture = pixel;
	setDimensions(pos, size);
}

RectangleSprite::~RectangleSprite() {
}

const Vector2 RectangleSprite::getSize() const {
	return Vector2(width, height);
}


void RectangleSprite::moveBy(const Vector2& moveVector) {

	position += moveVector;
	hitArea->position = position;
}



/**** ***** RECTANGLE FRAME START ***** ****/
RectangleFrame::RectangleFrame(GraphicsAsset* pixelAsset) {

	pixel = pixelAsset->getTexture();
}


RectangleFrame::~RectangleFrame() {
}

void RectangleFrame::setDimensions(const Vector2& pos, const Vector2& size,
	int frmThcknss) {

	frameThickness = frmThcknss;
	Vector2 position = pos;


	// upper horizontal frame
	frameHorizontal.left = 0;
	frameHorizontal.top = 0;
	frameHorizontal.right = size.x * scale.x;
	frameHorizontal.bottom = frameThickness; // thickness of frame
	frameTopPos = Vector2(position.x, position.y);


	// lower horizontal frame
	int height = size.y * scale.y;
	frameBottomPos = frameTopPos;
	frameBottomPos.y += height - frameThickness;
	// frame sticks out passed rectangle area; (-frameThickness) pulls it back in

	// left vertical frame
	frameLeftPos = frameBottomPos;
	frameLeftPos.y = position.y;
	frameVertical = frameHorizontal;
	frameVertical.right = frameThickness;
	frameVertical.bottom = height * scale.y;


	// right vertical frame
	frameRightPos = frameLeftPos;
	frameRightPos.x += size.x - frameThickness;
	// frame sticks out passed rectangle area; (-frameThickness) pulls it back in

	hitArea.reset(new HitArea(pos, size * scale));

}

bool cyberGrow = true;
void RectangleFrame::refreshDimensions() {

	if (!cyberGrow) {

		frameBottomPos = frameTopPos;
		frameBottomPos.y += getHeight() *scale.y - frameThickness;
		frameLeftPos = frameTopPos;
		frameRightPos = frameTopPos;
		frameRightPos.x += getWidth() *scale.x - frameThickness;

	}
	hitArea->size = Vector2(getWidth()*scale.x, getHeight()*scale.y);
	hitArea->position = frameTopPos;

}


void RectangleFrame::draw(SpriteBatch* batch) {

	// draw top horizontal bar
	batch->Draw(pixel.Get(), frameTopPos, &frameHorizontal,
		tint, rotation, origin, scale,
		SpriteEffects_None, 0.0f);

	// draw bottom horizontal bar
	batch->Draw(pixel.Get(), frameBottomPos, &frameHorizontal,
		tint, rotation, origin, scale,
		SpriteEffects_None, 0.0f);

	// draw left vertical bar
	batch->Draw(pixel.Get(), frameLeftPos, &frameVertical,
		tint, rotation, origin, scale,
		SpriteEffects_None, 0.0f);

	// draw right vertical bar
	batch->Draw(pixel.Get(), frameRightPos, &frameVertical,
		tint, rotation, origin, scale,
		SpriteEffects_None, 0.0f);
}

void RectangleFrame::setPosition(const Vector2& newPosition) {

	frameTopPos = newPosition;
	frameBottomPos = newPosition;
	frameBottomPos.y += getHeight() - frameThickness;
	frameLeftPos = newPosition;
	frameRightPos = newPosition;
	frameRightPos.x += getWidth() - frameThickness;
	hitArea->position = newPosition;
}

void RectangleFrame::moveBy(const Vector2& moveVector) {

	frameTopPos += moveVector;
	frameBottomPos += moveVector;
	frameLeftPos += moveVector;
	frameRightPos += moveVector;
	hitArea->position += moveVector;
}


const Vector2& RectangleFrame::getPosition() const {
	return frameTopPos;
}

const int RectangleFrame::getWidth() const {
	return frameHorizontal.right;
}

const int RectangleFrame::getHeight() const {
	return frameVertical.bottom;
}

const int RectangleFrame::getThickness() const {
	return frameThickness;
}

const Vector2& RectangleFrame::getOrigin() const {
	return origin;
}

void RectangleFrame::setTint(const XMFLOAT4 color) {
	tint = color;
}

const Vector2 & RectangleFrame::getScale() const {
	return scale;
}

const float RectangleFrame::getRotation() const {
	return rotation;
}

const Color & RectangleFrame::getTint() const {
	return tint;
}

const float RectangleFrame::getAlpha() const {
	return tint.w;
}

void RectangleFrame::setOrigin(const Vector2& orgn) {
	origin = orgn;
}

void RectangleFrame::setScale(const Vector2& scl) {

	scale = scl;
	refreshDimensions();
}

void RectangleFrame::setRotation(const float rot) {
	rotation = rot;
}

void RectangleFrame::setAlpha(const float alpha) {
	tint.w = alpha;
}

bool RectangleFrame::contains(const Vector2& point) {

	return hitArea->contains(point);
}
/** **** END RECTANGLEFRAME ****** **/





Line::Line(GraphicsAsset* pixelAsset) {

	pixel = pixelAsset->getTexture();
}

Line::Line(GraphicsAsset* pixelAsset,
	const Vector2& pos, const Vector2& size) {

	pixel = pixelAsset->getTexture();
	setDimensions(pos, size);
}

Line::~Line() {
}

void Line::setDimensions(const Vector2& pos, const Vector2& size) {

	position = pos;
	/*position.x -= size.x / 2;
	position.y -= size.y / 2;*/

	lineRect.left = 0;
	lineRect.top = 0;
	lineRect.right = size.x;
	lineRect.bottom = size.y;
}

void Line::setTint(const Color& color) {
	tint = color;

}

void Line::draw(SpriteBatch* batch) {

	batch->Draw(pixel.Get(), position, &lineRect,
		tint, 0.0f, Vector2(0, 0), scale,
		SpriteEffects_None, 0.0f);
}




TriangleFrame::TriangleFrame(GraphicsAsset* pixelAsset) {

	pixel = pixelAsset->getTexture();
}

TriangleFrame::~TriangleFrame() {
}

void TriangleFrame::setDimensions(const Vector2& p1, const Vector2& p2,
	const Vector2& p3, USHORT thickness) {

	point1 = p1;
	point2 = p2;
	point3 = p3;

	angle1to2 = atan2(point1.y - point2.y, point1.x - point2.x) + XM_PI;
	angle2to3 = atan2(point2.y - point3.y, point2.x - point3.x) + XM_PI;
	angle3to1 = atan2(point3.y - point1.y, point3.x - point1.x) + XM_PI;


	originLine1 = Vector2::Zero;
	originLine2 = originLine1;
	originLine3 = originLine1;


	Vector2 diff = point1 - point2;
	float length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT1.left = 0;
	lineRECT1.top = 0;
	lineRECT1.right = length;
	lineRECT1.bottom = thickness;

	diff = point2 - point3;
	length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT2.left = 0;
	lineRECT2.top = 0;
	lineRECT2.right = length;
	lineRECT2.bottom = thickness;

	diff = point3 - point1;
	length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT3.left = 0;
	lineRECT3.top = 0;
	lineRECT3.right = length;
	lineRECT3.bottom = thickness;
}

void TriangleFrame::draw(SpriteBatch* batch) {

	batch->Draw(pixel.Get(), point1, &lineRECT1,
		Color(0, 0, 0), angle1to2, originLine1, scale,
		SpriteEffects_None, 0.0f);

	batch->Draw(pixel.Get(), point2, &lineRECT2,
		Color(0, .5, 1), angle2to3, originLine2, scale,
		SpriteEffects_None, 0.0f);

	batch->Draw(pixel.Get(), point3, &lineRECT3,
		Color(1, .5, .5), angle3to1, originLine3, scale,
		SpriteEffects_None, 0.0f);
}


const Vector2& TriangleFrame::getPosition() const {
	return Vector2::Zero;
}

const Vector2& TriangleFrame::getOrigin() const {
	return origin;
}

const Vector2& TriangleFrame::getScale() const {
	return scale;
}

const float TriangleFrame::getRotation() const {
	return rotation;
}

const Color& TriangleFrame::getTint() const {
	return tint;
}

const float TriangleFrame::getAlpha() const {
	return 0.0f;
}

const int TriangleFrame::getWidth() const {
	return 0;
}

const int TriangleFrame::getHeight() const {
	return 0;
}

void TriangleFrame::moveBy(const Vector2& moveVector) {

	point1 += moveVector;
	point2 += moveVector;
	point3 += moveVector;
}

void TriangleFrame::setPosition(const Vector2& newPosition) {

	Vector2 diff = newPosition - point1;
	moveBy(diff);

}

void TriangleFrame::setOrigin(const Vector2& orgn) {
	origin = orgn;
}

void TriangleFrame::setScale(const Vector2& scl) {

	Vector2 origpos = point1;

	setPosition(Vector2::Zero);
	point2 *= scl;
	point3 *= scl;

	Vector2 diff = point1 - point2;
	float length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT1.right = length;
	lineRECT1.bottom *= scl.x;

	diff = point2 - point3;
	length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT2.right = length;
	lineRECT2.bottom *= scl.x;

	diff = point3 - point1;
	length = sqrt(diff.x * diff.x + diff.y * diff.y);
	lineRECT3.left = 0;
	lineRECT3.top = 0;
	lineRECT3.right = length;
	lineRECT3.bottom *= scl.x;

	setPosition(origpos);
}

void TriangleFrame::setRotation(const float rotation) {
}

void TriangleFrame::setTint(const XMFLOAT4 color) {
}

void TriangleFrame::setAlpha(const float alpha) {
}


