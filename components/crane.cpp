//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "Crane.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TCrane::TCrane(String imageUUID, TCanvas *canvas) {
	this->canvas = canvas;

	TImg *imageCarriage = DataModuleMP->GetImage(imageUUID);
	if (imageCarriage) {
		bitmapCarriage = imageCarriage->bitmap;
	} else {
		//load from application resource (default crane carriage)
		TBitmap *defaultCraneValue = new TBitmap;
		defaultCraneValue->LoadFromResourceName(NULL,  L"ID_BMP_CRANE_CARRIAGE");
		bitmapCarriage = defaultCraneValue;
	}

	bitmapCarriage->Transparent = true;

	balkBaseOffset.X = 0;
	balkBaseOffset.Y = 0;

	carriageBaseOffset.X = 0;
	carriageBaseOffset.Y = 0;

	arrowBaseOffset.X = 0;
	arrowBaseOffset.Y = 0;

	metersToPixelsX = 1.0;
	metersToPixelsY = 1.0;
	metersToDegree = 1.0;

	balkWidth = 20;

	balkHeight = 800;

	isArrow = false;

	visible = true;
}

void TCrane::setBalkBaseOffset(int xOffset, int yOffset) {
	balkBaseOffset.X = xOffset;
	balkBaseOffset.Y = yOffset;
}

void TCrane::setCarriageBaseOffset(int xOffset, int yOffset) {
	carriageBaseOffset.X = xOffset;
	carriageBaseOffset.Y = yOffset;
}

void TCrane::setArrowBaseOffset(int xOffset, int yOffset) {
	arrowBaseOffset.X = xOffset;
	arrowBaseOffset.Y = yOffset;
}

TCrane::setCoordinateResolution(double metersToPixelsX, double metersToPixelsY, double metersToDegree){
	this->metersToPixelsX = metersToPixelsX;
	this->metersToPixelsY = metersToPixelsX;
	this->metersToDegree = metersToDegree;
}

void TCrane::move(double x, double y) {
	position.X = x;
	position.Y = y;
}

void TCrane::move(TPoint positionPixels) {
	moveX(positionPixels.X);
	moveY(positionPixels.Y);
}

void TCrane::move(TPointF positionMeters) {
	move(positionMeters.X, positionMeters.Y);
}

void TCrane::moveXMeters(double xMeters) {
	move(xMeters, position.Y);
}

void TCrane::moveYMeters(double yMeters) {
	move(position.X, yMeters);
}

void TCrane::moveX(int xPixels) {
	position.X = convertToMeters(xPixels, AXIS_X);
}

void TCrane::moveY(int yPixels) {
	position.Y = convertToMeters(yPixels, AXIS_Y);
}

void TCrane::drawArrow(){
	if (!isArrow) {
		return;
	}

	int xPos = arrowBaseOffset.X + carriageBaseOffset.X + balkBaseOffset.X + convertToPixels(position.X, AXIS_X);
	int yPos = arrowBaseOffset.Y + carriageBaseOffset.Y + balkBaseOffset.Y + convertToPixels(position.Y, AXIS_Y);

	double arrowLength = 50;

	double rotationDegree = metersToDegree * rotationMeters;

	int x2Pos = xPos + arrowLength * cos(rotationDegree * M_PI / 180.0);
	int y2Pos = yPos + arrowLength * sin(rotationDegree * M_PI / 180.0);

	canvas->Pen->Width = 5;
	canvas->MoveTo(xPos, yPos);
	canvas->LineTo(x2Pos, y2Pos);
	/*
	arrow->Picture->Bitmap->Transparent = true;
	arrow->Picture->Bitmap->TransparentColor = clWhite;
	arrow->Picture->Bitmap->Canvas->Pen->Color = clBlack;
	arrow->Picture->Bitmap->Canvas->Pen->Width = 10;
	arrow->Picture->Bitmap->Canvas->MoveTo(0, 0);
	arrow->Picture->Bitmap->Canvas->LineTo(100, 100);
	arrow->Picture->Bitmap->Canvas->Draw(0,0, arrow->Picture->Bitmap);
	*/
}

void TCrane::drawBalk(){
	int xPos = balkBaseOffset.X + convertToPixels(position.X, AXIS_X);
	int yPos = balkBaseOffset.Y;

	canvas->Brush->Color = clBlack;
	canvas->Rectangle(xPos, yPos, xPos + balkWidth, yPos + balkHeight);
}

void TCrane::drawCarriage(){
	int xPos = carriageBaseOffset.X + balkBaseOffset.X + convertToPixels(position.X, AXIS_X);
	int yPos = carriageBaseOffset.Y + balkBaseOffset.Y + convertToPixels(position.Y, AXIS_Y);

	canvas->Draw(xPos, yPos, bitmapCarriage);
}

void TCrane::draw(){
	if (!visible) {
		return;
	}
	/*
	int xPos = balkBaseOffset.X + convertToPixels(position.X, TCraneAxis::x);
	int yPos = balkBaseOffset.Y + convertToPixels(position.Y, TCraneAxis::y);

	balk->Left = xPos;

	carriage->Left = xPos + carriageBaseOffset.X;
	carriage->Top = yPos + carriageBaseOffset.Y;
	*/
	drawBalk();
	drawCarriage();
	drawArrow();
}

double TCrane::convertToMeters(int pixels, TCraneAxis axis) {
	if (axis == AXIS_X) {
		return pixels / metersToPixelsX;
	} else if (axis == AXIS_Y) {
		return pixels / metersToPixelsY;
	}
}

TPointF TCrane::convertToMeters(TPoint point) {
	double x = convertToMeters(point.X, AXIS_X);
	double y = convertToMeters(point.Y, AXIS_Y);
	return TPointF(x, y);
}

TCrane::convertToPixels(double distance, TCraneAxis axis) {
	if (axis == AXIS_X) {
		return distance * metersToPixelsX;
	} else if (axis == AXIS_Y) {
    	return distance * metersToPixelsY;
	}
}

const TPointF TCrane::getPosition() {
	return position;
}

double TCrane::getRotationDegree() {
	return metersToDegree * rotationMeters;
}

double TCrane::getRotationMeters() {
	return rotationMeters;
}


const TPoint TCrane::getPositionPixels() {
	return TPoint(
		convertToPixels(position.x, AXIS_X),
		convertToPixels(position.y, AXIS_Y));
}

void TCrane::displayArrow(bool isArrow) {
	this->isArrow = isArrow;
}

void TCrane::setArrowCraneBaseOffset(int xOffset, int yOffset) {
	arrowBaseOffset.X = xOffset;
	arrowBaseOffset.Y = yOffset;
}

void TCrane::rotateAsDegree(double rotationDegree) {
	rotationMeters = rotationDegree / metersToDegree;
}

void TCrane::rotateAsMeters(double meters) {
	rotationMeters = meters;
}

void TCrane::setCanvas(TCanvas *canvas) {
	this->canvas = canvas;
}

void TCrane::setBalkHeight(int balkHeight) {
	this->balkHeight = balkHeight;
}

void TCrane::setBalkWidth(int balkWidth) {
	this->balkWidth = balkWidth;
}

int TCrane::getBalkWidth() {
	return balkWidth;
}

int TCrane::getBalkHeight() {
	return balkHeight;
}

void TCrane::setVisible(bool visible) {
	this->visible = visible;
}

bool TCrane::getVisible() {
	return visible;
}
