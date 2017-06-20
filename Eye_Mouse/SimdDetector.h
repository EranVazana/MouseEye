#pragma once
#include <vector>

#include "opencv2/opencv.hpp"
#ifndef SIMD_OPENCV_ENABLE
#define SIMD_OPENCV_ENABLE
#endif
#include "Simd/SimdLib.hpp"
#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"

#include "Errors_And_Warnings.h"

using namespace std;

//-Face and eyes detection class used with Simd haar-cascades optimization.
//-The Simd libary provide faster haar-cascade base detection than the built-in functions in OpenCV.
//-Simd Github: https://github.com/ermig1979/Simd
class SimdDetector {
public:
	#define DETECTION_SCALE_FACTOR 1.2

	#define DETECTION_MIN_FACE_SIZE cv::Size(150, 150)

	#define DETECTION_MIN_EYE_SIZE cv::Size(45, 45)
	#define DETECTION_MAX_EYE_SIZE cv::Size(70, 70)


	SimdDetector();
	~SimdDetector();

	void Init(cv::Size frame_size);

	vector<cv::Rect> getFaces(cv::Mat);
	vector<cv::Rect> getEyes(cv::Mat);

private:
	#define FACE_CASCADE_NAME "Cascades/haar_face_0.xml"
	#define EYES_CASCADE_NAME "Cascades/haarcascade_eye.xml"

	enum DetectorID : bool {FACE_DETECTION = true, EYES_DETECTION = false};

	vector<cv::Rect> getObjects(cv::Mat, DetectorID);

	typedef Simd::Detection<Simd::Allocator> Detector;
	Detector _face_detector;
	Detector _eyes_detector;
};