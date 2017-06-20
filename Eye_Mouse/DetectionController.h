#pragma once
#include <Windows.h>

#include "opencv2/highgui.hpp"

#include "Windows_Settings.h"
#include "Database.h"
#include "Desktop.h"

using namespace cv;
using namespace DatabaseConstants;

//-Constants variables for the controller.
namespace DetectionControllerConstants {
	//-Max values of the controller settings.
	enum ControllerMaxValues { BRIGHTNESS_MAX = 100, THRESHOLD_MAX = 255, CONTRAST_MAX = 10, BLUR_MAX = 31, EYEBROW_REMOVE_MAX = 8, SENSETIVITY_MAX = 20 };

	#define BRIGHTNESS_TEXT "Brightness"
	#define CONTRAST_TEXT "Contrast"
	#define THRESHOLD_TEXT "Threshold"
	#define BLUR_KSIZE_X_TEXT "Blur K-X"
	#define BLUR_KSIZE_Y_TEXT "Blur K-Y"
	#define EYEBROW_REMOVE_TEXT "EB Removal"
	#define FACE_SENSETIVITY_TEXT "Face sens"
	#define EYE_SENSETIVITY_TEXT "Eye sens"
}

//-Class used for representing the user detection values and creating a GUI for the controller.
class DetectionController{
public:
	DetectionController(Database &);
	~DetectionController();

	void open();
	void close();
	void reset();
	void saveSettings();

	int _brightness;
	int _contrast;
	int _threshold;
	int _blur_ksize_X, _blur_ksize_Y;
	int _eyebrow_remove;

	int _face_position_sensetivity;
	int _eyes_position_sensetivity;

private:
	bool isWindowOpen();

	Database _db;
};