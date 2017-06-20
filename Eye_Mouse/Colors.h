#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;

typedef Scalar cvRGB;

//-Static class for OpenCV colors.
class Colors {
public:
	#define CV_RED CV_RGB(255, 0, 0)
	#define CV_GREEN CV_RGB(0, 255, 0)
	#define CV_BLUE CV_RGB(0, 0, 255)

	#define CV_CYAN CV_RGB(0, 255, 255)
	#define CV_PINK CV_RGB(255, 0, 128)
	#define CV_YELLOW CV_RGB(255, 255, 0)
	#define CV_ORANGE CV_RGB(255, 128, 64)

	#define CV_WHITE CV_RGB(255, 255, 255)
	#define CV_GRAY CV_RGB(128, 128, 128)
	#define CV_BLACK CV_RGB(0, 0, 0)
};