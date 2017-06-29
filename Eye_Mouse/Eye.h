#pragma once
#include <opencv2\opencv.hpp>

#include "Colors.h"

using namespace cv;

//-Enum of sides.
enum EyeSide : bool { RIGHT = true, LEFT = false };

//-Class used to represent the user eyes values.
class Eye {
public:
	Eye(Rect, Point, EyeSide);
	Eye();
	Eye(const Eye &);
	~Eye();

	unsigned int getEyeRadius();

	void draw(Mat);
	bool isEmpty();

	operator std::string();

	Rect _eye_rect;
	Point _pupil;
	EyeSide _side;

private:
	#define RIGHT_EYE_MARK CV_RED
	#define LEFT_EYE_MARK CV_CYAN

	#define PUPIL_RADIUS 1
	#define PUPIL_THICKNESS 2
};