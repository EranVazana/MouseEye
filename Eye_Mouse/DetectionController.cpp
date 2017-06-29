#include "DetectionController.h"

//======================================================================

using namespace DetectionControllerConstants;

//-Constructor.
DetectionController::DetectionController(Database &db) : _db(db){
	//-Init settings.
	_brightness = _db.getSetting(BRIGHTNESS_NAME);
	_threshold = _db.getSetting(THRESHOLD_NAME);
	_contrast = _db.getSetting(CONTRAST_NAME);
	_blur_ksize_X = _db.getSetting(BLUR_KSIZE_X_NAME);
	_blur_ksize_Y = _db.getSetting(BLUR_KSIZE_Y_NAME);
	_eyebrow_remove = _db.getSetting(EYEBROW_REMOVE_NAME);
	_face_position_sensetivity = _db.getSetting(FACE_POSITION_SENSETIVITY_NAME);
	_eyes_position_sensetivity = _db.getSetting(EYES_POSITION_SENSETIVITY_NAME);
}

//-Destructor.
DetectionController::~DetectionController() {
	close();
}

//-Open the controller window.
void DetectionController::open() {
	//-check if window is already open.
	if (isWindowOpen())
		return;

	//-Init the controler window.
	namedWindow(CONTROLLER_WINDOW_NAME);
	resizeWindow(CONTROLLER_WINDOW_NAME, CONTROLLER_WINDOW_SIZE.width, CONTROLLER_WINDOW_SIZE.height);
	moveWindow(CONTROLLER_WINDOW_NAME, CONTROLLER_WINDOW_LOCATION.x, CONTROLLER_WINDOW_LOCATION.y);
	Desktop::SetIcon(CONTROLLER_WINDOW_NAME);

	//-Create a trackbar for every setting.
	createTrackbar(BRIGHTNESS_TEXT, CONTROLLER_WINDOW_NAME, &_brightness, BRIGHTNESS_MAX);
	createTrackbar(CONTRAST_TEXT, CONTROLLER_WINDOW_NAME, &_contrast, CONTRAST_MAX);
	createTrackbar(THRESHOLD_TEXT, CONTROLLER_WINDOW_NAME, &_threshold, THRESHOLD_MAX);
	createTrackbar(BLUR_KSIZE_X_TEXT, CONTROLLER_WINDOW_NAME, &_blur_ksize_X, BLUR_MAX);
	createTrackbar(BLUR_KSIZE_Y_TEXT, CONTROLLER_WINDOW_NAME, &_blur_ksize_Y, BLUR_MAX);
	createTrackbar(EYEBROW_REMOVE_TEXT, CONTROLLER_WINDOW_NAME, &_eyebrow_remove, EYEBROW_REMOVE_MAX);

	createTrackbar(FACE_SENSETIVITY_TEXT, CONTROLLER_WINDOW_NAME, &_face_position_sensetivity, SENSETIVITY_MAX);
	createTrackbar(EYE_SENSETIVITY_TEXT, CONTROLLER_WINDOW_NAME, &_eyes_position_sensetivity, SENSETIVITY_MAX);
}

//-Save the settings to the Database and close the window.
void DetectionController::close() {
	//-Save the user setiings
	saveSettings();

	//-Close the controller window.
	destroyWindow(CONTROLLER_WINDOW_NAME);
}

//-Reset every setting to its defualt value.
void DetectionController::reset() {
	_brightness = DEFAULT_BRIGHTNESS;
	setTrackbarPos(BRIGHTNESS_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_BRIGHTNESS);

	_contrast = DEFAULT_CONTRAST;
	setTrackbarPos(CONTRAST_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_CONTRAST);

	_threshold = DEFAULT_THRESHOLD;
	setTrackbarPos(THRESHOLD_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_THRESHOLD);

	_blur_ksize_X = DEFAULT_BLUR_KSIZE_X;
	setTrackbarPos(BLUR_KSIZE_X_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_BLUR_KSIZE_X);

	_blur_ksize_Y = DEFAULT_BLUR_KSIZE_Y;
	setTrackbarPos(BLUR_KSIZE_Y_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_BLUR_KSIZE_Y);

	_eyebrow_remove = DEFAULT_EYEBROW_REMOVE;
	setTrackbarPos(EYEBROW_REMOVE_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_EYEBROW_REMOVE);

	_face_position_sensetivity = DEFAULT_FACE_POSITION_SENSETIVITY;
	setTrackbarPos(FACE_SENSETIVITY_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_FACE_POSITION_SENSETIVITY);

	_eyes_position_sensetivity = DEFAULT_EYES_POSITION_SENSETIVITY;
	setTrackbarPos(EYE_SENSETIVITY_TEXT, CONTROLLER_WINDOW_NAME, DEFAULT_EYES_POSITION_SENSETIVITY);
}

//-Save the settings in the database.
void DetectionController::saveSettings() {
	//-Update every setting in the database.
	_db.changeSetting(BRIGHTNESS_NAME, _brightness);
	_db.changeSetting(THRESHOLD_NAME, _threshold);
	_db.changeSetting(CONTRAST_NAME, _contrast);
	_db.changeSetting(BLUR_KSIZE_X_NAME, _blur_ksize_X);
	_db.changeSetting(BLUR_KSIZE_Y_NAME, _blur_ksize_Y);
	_db.changeSetting(EYEBROW_REMOVE_NAME, _eyebrow_remove);
	_db.changeSetting(FACE_POSITION_SENSETIVITY_NAME, _face_position_sensetivity);
	_db.changeSetting(EYES_POSITION_SENSETIVITY_NAME, _eyes_position_sensetivity);
}

// Output: True if window is open, false otherwise.
//-Checks if the controller window is open.
bool DetectionController::isWindowOpen() {
	return cvGetWindowHandle(CONTROLLER_WINDOW_NAME) != nullptr;
}