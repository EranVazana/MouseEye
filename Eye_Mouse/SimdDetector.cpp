#include "SimdDetector.h"

//======================================================================

//-Constuctor.
SimdDetector::SimdDetector() {
	//-Load the face cascade.
	if (!_face_detector.Load(FACE_CASCADE_NAME))
		throw FaceCascadeInitException();

	//-Load the eyes cascade.
	if (!_eyes_detector.Load(EYES_CASCADE_NAME))
		throw EyesCascadeInitException();
}

//-Destructor.
SimdDetector::~SimdDetector() {

}

// Input: Size of the user's device resolution.
//-Initalize the face detector.
void SimdDetector::Init(cv::Size frame_size) {
	_face_detector.Init(frame_size, DETECTION_SCALE_FACTOR, DETECTION_MIN_FACE_SIZE);
}

// Input: Frame from the user's device.
// Output: Vector of all posibble option for the user's face in the frame.
//-Detects faces on the frame.
vector<cv::Rect> SimdDetector::getFaces(cv::Mat frame) {
	return getObjects(frame, FACE_DETECTION);
}

// Input: ROI of the user's face.
// Output: Vector of all posibble option for the user's eyes in the frame.
//-Detects eyes on the face ROI.
vector<cv::Rect> SimdDetector::getEyes(cv::Mat faceROI) {
	//-Init the eye detector for the new ROI size.
	_eyes_detector.Init(faceROI.size(), DETECTION_SCALE_FACTOR, DETECTION_MIN_EYE_SIZE, DETECTION_MAX_EYE_SIZE);

	return getObjects(faceROI, EYES_DETECTION);
}

// Input: Frame from the user's device, indicator of which body part to detect.
// Output: Vector of Rects that contain the values of the found objects.
//-Detects the objects in the frame.
vector<cv::Rect> SimdDetector::getObjects(cv::Mat src, DetectorID detector) {
	//-Find objects in the frame.
	Detector::Objects simd_objects;
	detector ? _face_detector.Detect(src, simd_objects) : _eyes_detector.Detect(src, simd_objects);

	//-Transform the Simd objects to OpenCV Rect objects.
	vector<cv::Rect> cv_objects;
	for (unsigned int i = 0; i < simd_objects.size(); ++i)
		cv_objects.push_back(simd_objects[i].rect);

	//-Return the found objects.
	return cv_objects;
}