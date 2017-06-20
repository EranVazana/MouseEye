#pragma once
#include "CV_OperatorOverloading.h"

//-Static class that contain the settings of the program windows.
class Windows_Settings {
public:
	#define SETTINGS_CALIBRATION_MAIN_WINDOW_NAME "MousEye Project - Eran Vazana And Alon Wasserman" //-For marking eyes values location.
	#define SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION (Point((int)(GetSystemMetrics(SM_CXSCREEN) / 2), 100) - XY_Amount(320, 0))

	#define CONTROLLER_WINDOW_NAME "Eye Detection Controller" //-Eye detection control.
	#define CONTROLLER_WINDOW_LOCATION (SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION + XY_Amount(655, 0))
	#define CONTROLLER_WINDOW_SIZE Size(350,350)

	#define RATIO_CALIBRATION_MAIN_WINDOW_NAME "Gaze Calibration" //-For setting eye to mouse ratio.

	#define ZOOM_AREA_GUI_WINDOW_NAME "Area Selection" //-Zoom Area Selection GUI.
	#define SCREEN_ZOOM_WINDOW_NAME "Zoomed Area" //-Zoomed area window.

	#define RIGHT_EYE_PREVIEW_WINDOW_NAME "Right Eye Settings Preview" //-For viewing right pupil detction setting.
	#define RIGHT_EYE_PREVIEW_WINDOW_LOCATION (SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION - XY_Amount(150, 0))
	#define RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME "Right Pupil Detection Preview" //-For viewing right pupil detection. 
	#define RIGHT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION (RIGHT_EYE_PREVIEW_WINDOW_LOCATION + XY_Amount(0, 60))

	#define LEFT_EYE_PREVIEW_WINDOW_NAME "Left Eye Settings Preview" //-For viewing left pupil detction setting.
	#define LEFT_EYE_PREVIEW_WINDOW_LOCATION (SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION - XY_Amount(90, 0))
	#define LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME "Left Pupil Detection Preview" //-For viewing left pupil detection. 
	#define LEFT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION (LEFT_EYE_PREVIEW_WINDOW_LOCATION + XY_Amount(0, 60))

	#define EYE_PREVIEW_DEFAULT_SIZE Size(45,45)

	//-DEBUG
	#define MOUSE_TESTING_WINDOW_NAME "Mouse Testing" //-For testing mouse movement.
};