#pragma once
#include "Text.h"
#include "CV_OperatorOverloading.h"

//-Static class that contain the GUI strings values.
class Texts {
private:
	#define GO_BACK_LOCATION Point(10, 15)
	#define GO_BACK_SCALE 0.5

	#define CURRENT_CALIBRATED_EYE_TEXT_LOCATION (CALIBRATED_EYE_TEXT._location + XY_Amount(280, 0))
	#define CURRENT_CALIBRATED_EYE_TEXT_SCALE 1.0

public:
	#define OPEN_CONTROLLER_TEXT Text("Press 'E' to open the controller.", Point(350, 15))
	#define RESET_CONTROLLER_TEXT Text("Press 'R' to reset the controller.", Point(350, 35))

	#define CONTINUE_TEXT Text("Press 'Enter' to enter mouse calibration.", Point(5,475))

	//======================================================================

	#define GO_BACK_RATIO_TEXT Text("Press 'Backspace' to go back to detection settings.", GO_BACK_LOCATION, GO_BACK_SCALE, CV_BLACK)
	#define GO_BACK_MOUSE_CONTROL_TEXT Text("Press 'Backspace' to go back to ratio settings.", GO_BACK_LOCATION, GO_BACK_SCALE, CV_BLACK)

	#define NO_EYE_WARNING_TEXT Text("No Eyes Detected!", Point((unsigned int)(GetSystemMetrics(SM_CXSCREEN) * 0.375), (unsigned int)(GetSystemMetrics(SM_CYSCREEN) * 0.2085)))

	#define CALIBRATED_EYE_TEXT Text("Calibrated eye: ", NO_EYE_WARNING_TEXT._location + XY_Amount(30, 600), CURRENT_CALIBRATED_EYE_TEXT_SCALE, CV_BLACK)
	
	#define RIGHT_EYE_TEXT Text("Right eye", CURRENT_CALIBRATED_EYE_TEXT_LOCATION, CURRENT_CALIBRATED_EYE_TEXT_SCALE, CV_RED)
	#define LEFT_EYE_TEXT Text("Left eye", CURRENT_CALIBRATED_EYE_TEXT_LOCATION, CURRENT_CALIBRATED_EYE_TEXT_SCALE ,CV_CYAN)

	#define CHANGE_CALIBRATED_EYE_TEXT Text("Press 'SPACE' to change eye.", CALIBRATED_EYE_TEXT._location + XY_Amount(100, 30), DEFAULT_TEXT_SCALE, CV_BLACK)
};