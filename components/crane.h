// ---------------------------------------------------------------------------

#ifndef CraneH
#define CraneH

// ---------------------------------------------------------------------------

enum TCraneAxis {AXIS_X, AXIS_Y};

//TODO implement IMnemoshemaView

class TCrane {
private:
	//relation with sensor and how to get new coorditaion
	//const TSensor *sensor;

	bool visible;

	int balkWidth;

	int balkHeight;

	//bool flag is crane has arrow
	bool isArrow;

	//canvas for draw balk1, balk2, carriage1, carriage2, crane arrow
	TCanvas *canvas;

	TBitmap *bitmapCarriage;

	TBitmap *bitmapBackground;

	//image for vertical balk
	//TImage *balk;

	//image for crane
	//TImage *carriage;

	//image bitmap for arrow
	//TImage *arrow;

	//base offset of top left balk coordinate
	TPoint balkBaseOffset;

	//base offset of top left crane related with balk
	TPoint carriageBaseOffset;

	//base offset for crane arrow
	TPoint arrowBaseOffset;

	//current crane position meters (getting from some kind of crane sensor)
	TPointF position;

	//current angle crane position
	//double rotationDegree;

	//current rotation meters of crane (getting from some kind of crane sensor)
	double rotationMeters;

	//coefficient for convert x meters into x pixels position
	double metersToPixelsX;

	//coefficient for convert y meters into y pixels position
	double metersToPixelsY;

	//coefficient for convert meters into degree
	double metersToDegree;

	//draw crane arrow, using current position and direction
	void drawArrow();

	void drawBalk();

	void drawCarriage();

public:
	TCrane(String imageUUID, TCanvas *canvas = NULL/*, TSensor *sensor*/);

	/**
	move crane to specify position in meters
	*/
	void move(double xMeters, double yMeters);

	void move(TPointF positionMeters);

	void move(TPoint positionPixels);

	void moveXMeters(double xMeters);

	void moveYMeters(double yMeters);

	/**
	move crane by xPixels for x-axis

	*/
	void moveX(int xPixels);

	/**
	move crabe by yPixels for y-axis
	*/
	void moveY(int xPixels);

	/**
	initialize the left top available position for the balk, this position
	will be set, where invoke move(0, 0) position

	@param {xOffset} pixels offset of x coordinate
	@param {yOffset} pixels offset of y coordinate
	*/
	void setBalkBaseOffset(int xOffset, int yOffset);

	/**
	initialize the left top available position of the crane carriage related
	to balk

	@param {xOffset} pixels offset  of x coordinate
	@param {yOffset} pixels offset of y coordinate
	*/
	void setCarriageBaseOffset(int xOffset, int yOffset);

	/**
	@param {xOffset} pixels offset
	@param {yOffset} pixels offset
	*/
	void TCrane::setArrowBaseOffset(int xOffset, int yOffset);

	/**
	specify coordinate resolution
	*/
	setCoordinateResolution(double metersToPixelsX, double metersToPixelsY, double metersToDegree);

	/**
	convert pixels distance into meters
	@param {axis} this is enum TCraneAxis {x, y};
	*/
	double convertToMeters(int pixels, TCraneAxis axis);

	/**
	convert pixels distance into meters
	@param {TPoint} point of pixels coordinate
	return double, double meters coordinate
	*/
	TPointF convertToMeters(TPoint point);

	/**
	convert degree
	*/
//	double convertToMeters(double degree);

	/**
	convert meters distance into pixels
	@param {axis} this is enum TCraneAxis {x, y};
	*/
	int convertToPixels(double meters, TCraneAxis axis);

	/**
	return current crane position in meters
	*/
	const TPointF getPosition();

	/**
	return current crane position in pixels
	*/
	const TPoint getPositionPixels();

	double getRotationDegree();

	double getRotationMeters();

	/**
	draw whole crane using the last crane position
	*/
	void draw();

	void displayArrow(bool isArrow);

	/**
	@param {xOffset}
	@param {yOffset}
	*/
	void setArrowCraneBaseOffset(int xOffset, int yOffset);

	void rotateAsDegree(double angleDegree);

	void rotateAsMeters(double meters);

	void setCanvas(TCanvas *canvas);

	void setBalkWidth(int balkWidth);

	void setBalkHeight(int balkHeight);

	int getBalkWidth();

	int getBalkHeight();

	void setVisible(bool visible);

	bool getVisible();
};

#endif
