#pragma once
#include "opencv2/opencv.hpp"
#include <Windows.h>

using namespace cv;

//-Static class for all media related.
class Media {
public:
	static Mat _no_eye_image;
	static HICON _program_icon;

private:
	#define NEF_IMAGE_LOCATION "Media/NEF.png"
	#define PROGRAM_ICON_LOCATION "Media/MousEye.ico"
	#define ICON_SIZE 16
};