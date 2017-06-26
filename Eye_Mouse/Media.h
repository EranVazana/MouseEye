#pragma once
#include "opencv2/opencv.hpp"
#include <Windows.h>

//-Static class for all media related.
namespace Media {
	static cv::Mat _no_eye_image = cv::imread("Media/NEF.png");
	static HICON _program_icon = (HICON)LoadImage(NULL, "Media/MousEye.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
};