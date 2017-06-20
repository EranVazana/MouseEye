#pragma once
#include <string>

#include <opencv2\opencv.hpp>

#include "Colors.h"

using namespace cv;

//-Enum of sides.
enum EyeSide : bool { RIGHT = true, LEFT = false };

//-Class used to represent the user eyes values.
class Eye {
public:
	Eye(Rect, Rect, Point, Point, Point, int, EyeSide);
	Eye();
	Eye(const Eye &);
	~Eye();

	void draw(Mat);
	bool isEmpty();

	operator std::string();

	Rect _eye_rect;
	Rect _pupil_rect;
	Point _pupil, _eye_center, _corner;
	int _eye_radius;
	EyeSide _side;

private:
	#define RIGHT_EYE_MARK CV_RED
	#define LEFT_EYE_MARK CV_CYAN
	#define EYE_CORNER_MARK CV_YELLOW

	#define PUPIL_RADIUS 1
	#define PUPIL_THICKNESS 2
};