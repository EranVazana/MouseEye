#pragma once
//-Basic C++ libaries.
#include <string>
#include <fstream>

//-Containers.
#include <vector>
#include <array>

//-Opencv libary header files.
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"

//-CaptureDevice class.
#include "CaptureDevice.h"

//-Media class.
#include "Media.h"

//-Desktop class.
#include "Desktop.h"

//-Keyboard Hook class.
#include  "KeyboardHook.h"

//-Detection setting GUI. 
#include "DetectionController.h"

//-SimdDetector class.
#include "SimdDetector.h"

//-Identification class.
#include "UserIdentification.h"

//-FPSCounter class.
#include "FPSCounter.h"

//-OpenCV operator overloading.
#include "CV_OperatorOverloading.h"

//-Constant variables.
#include "Windows_Settings.h"
#include "Texts.h"

using namespace cv;

//-Return codes for the main function.
enum ReturnCode { DETECTION_SETTINGS_CALIBRATION = 0, GAZE_CALIBRATION, MOUSE_CONTROL, APPLICATION_EXIT };

//-Main class used for detecting eye values and calculating mouse movement.
class GazeTracker {
public:
	GazeTracker();
	~GazeTracker();
	ReturnCode calibrateDetectionSettings();
	ReturnCode calibrateGazeRatio();
	ReturnCode mouseControl();
	void mouseControlDevTest();

private:
	#define NUMBER_OF_CALIBRATION_POINTS 5
	typedef array<Point, NUMBER_OF_CALIBRATION_POINTS> CalibrationPoints;

	#define AREAS_PER_ROW 5
	typedef array<array<Rect, AREAS_PER_ROW>, AREAS_PER_ROW> GazeAreas;

	void initGazeAreas();

	HWND initUserDetectionWindows();
	HWND initFullscreenWindow(const char*);
	void removeWindowTitlebar(const char*);
	bool keepRunning(HWND);
	void closeWindow(HWND);

	void proceed(ReturnCode);

	void addBorders(Mat&);
	void printText(Mat, Text);
	void displayNoDetectionWarning(Mat, const char*, Text, ReturnCode);
	char getKey(unsigned int = 1);

	UserIdentification detectUserIdentification(Mat, bool = false, bool = false);
	Rect updatedRectPosition(Rect, Rect, int);
	EyeSide getSideOfEye(Rect, Rect);
	void blurImage(Mat&);
	void removeTopRows(Mat, unsigned int);

	Point getEyePovAvgPoint();
	void calibrate(UserIdentification, unsigned int&);
	void calibrationReport();

	CalibrationPoints getFixedCalibrationPoints(Point);
	unsigned int getAxisValue(CalibrationPoints, Point, unsigned int, bool);
	Point getEstimatedEyeGazeLocation(UserIdentification);
	Rect getGazeArea(Point);

	Mat zoomScreenImage(Rect);
	Point getCursorLocation(Rect);
	Point getZoomedCursorLocation(Rect, Rect);

	//======================================================================

	#define AVG_ITERATION_NUMBER 10

	#define EXIT_COMBINATION_KEYS CombinationKeys { L_SHIFT, ESC }

	ReturnCode _return_status_code;
	bool _keep_alive;
	
	Database _db;

	CaptureDevice _capture_device;

	Desktop _user_desktop;
	XY_Amount _borders;

	SimdDetector _simd_detector;

	DetectionController _detection_controller;
	enum KeyboardOptions { CONTINUE = 13 /* Enter Key */, SPACE = 32 /* Whitespace key */, BACK = 8 /* Backspace key */, QUIT = 27 /* ESC Key */, OPEN_CONTROLLER = 'e' , RESET_CONTROLLER = 'r', MIRROR_FRAME = 'f' };
	
	EyeSide _eye_to_calibrate;
	CalibrationPoints _base_screen_calibration_points;
	CalibrationPoints _user_calibration_points;
	Point _face_center_avg;
	enum UserCalibrationPointIndexs { TOP = 0, LEFT, MIDDLE, RIGHT, BOTTOM };
	
	GazeAreas _screen_gaze_areas;
};