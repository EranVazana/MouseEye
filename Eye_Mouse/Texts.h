#pragma once
#include "Colors.h"
#include "CV_OperatorOverloading.h"

//-Static class that contain the GUI strings values.
class Texts {
public:
	#define DEFAULT_TEXT_SCALE 0.5
	#define DEFAULT_TEXT_COLOR CV_GREEN

	//======================================================================

	#define FPS_INDICATION_TEXT "FPS: "
	#define FPS_INDICATION_TEXT_LOCATION Point(5,15)

	#define OPEN_CONTROLLER_TEXT "Press 'E' to open the controller."
	#define OPEN_CONTROLLER_TEXT_LOCATION Point(350, 15)

	#define RESET_CONTROLLER_TEXT "Press 'R' to reset the controller."
	#define RESET_CONTROLLER_TEXT_LOCATION Point(350, 35)

	#define CONTINUE_TEXT "Press 'Enter' to enter mouse calibration."
	#define CONTINUE_TEXT_LOCATION Point(5,475)

	//======================================================================

	#define GO_BACK_TEXT "Press 'Backspace' to go back to detection settings."
	#define GO_BACK_TEXT_LOCATION Point(10, 15)

	#define NO_EYE_WARNING_TEXT "No Eyes Detected!"
	#define NO_EYE_WARNING_TEXT_LOCATION Point((unsigned int)(GetSystemMetrics(SM_CXSCREEN) * 0.375), (unsigned int)(GetSystemMetrics(SM_CYSCREEN) * 0.2085))

	#define CALIBRATED_EYE_TEXT "Calibrated eye: "
	#define CALIBRATED_EYE_TEXT_LOCATION NO_EYE_WARNING_TEXT_LOCATION + XY_Amount(30, 600)
	#define CALIBRATED_EYE_SCALE 1.0

	#define RIGHT_EYE_TEXT "Right eye"
	#define LEFT_EYE_TEXT "Left eye"
	#define CURRENT_CALIBRATED_EYE_TEXT_LOCATION CALIBRATED_EYE_TEXT_LOCATION + XY_Amount(280, 0)

	#define CHANGE_CALIBRATED_EYE_TEXT "Press 'SPACE' to change eye."
	#define CHANGE_CALIBRATED_EYE_TEXT_LOCATION  CALIBRATED_EYE_TEXT_LOCATION + XY_Amount(100, 30)
};