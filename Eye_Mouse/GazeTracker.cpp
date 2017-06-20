#include "GazeTracker.h"

//======================================================================

//-Constructor.
GazeTracker::GazeTracker() : _detection_controller(_db) {
	//-Set program active value.
	_keep_alive = true;

	//-Open camera.
	initCaptureDevice();

	//-Get device resolution values.
	_capture_device_resolusion = Resolution((unsigned int)_capture_device.get(CAP_PROP_FRAME_WIDTH), 
											(unsigned int)_capture_device.get(CAP_PROP_FRAME_HEIGHT));

	//-Init the Simd Detector.
	_simd_detector.Init(_capture_device_resolusion);

	//-Set defualt mirror value.
	_mirror_frame = true;

	//-Get screen resolution values.
	unsigned int screen_width = GetSystemMetrics(SM_CXSCREEN);
	unsigned int screen_height = GetSystemMetrics(SM_CYSCREEN);

	//-Get borders values.
	_borders = XY_Amount((unsigned int)((screen_width - _capture_device_resolusion.width) / 2), 
						 (unsigned int)((screen_height - _capture_device_resolusion.height) / 2));

	//-Set side of eye to calibrate.
	_eye_to_calibrate = EyeSide::LEFT;

	//-Initalize calibration points cordinations.
	_base_screen_calibration_points = { Point((unsigned int)(screen_width / 2), 0),
										Point(0, (unsigned int)(screen_height / 2)), Point((unsigned int)(screen_width / 2), screen_height / 2), Point((unsigned int)(screen_width), screen_height / 2),
										Point((unsigned int)(screen_width / 2), screen_height) };

	//-Init gaze areas for the mouse.
	initGazeAreas();

	_return_status_code = APPLICATION_EXIT;
}

//-Destructor.
GazeTracker::~GazeTracker() {
	destroyAllWindows();
	_capture_device.release();
}

//-Try opening X amount of devices until one is open.
//-If no device was found, throw an exception to the main function.
void GazeTracker::initCaptureDevice() {
	for (unsigned int device = 0; device < MAX_DEVICES_NUMBER; ++device) {
		_capture_device.open(device);
		if (_capture_device.isOpened())
			return;
		_capture_device.release();
	}

	//-Throw exception if no device is open.
	throw exception(CANNOT_OPEN_CAPTURE_DEVICE);
}

//-Initialize the screen gaze areas.
void GazeTracker::initGazeAreas() {
	//-Calculate the width and height for each area based on the screen resolution values.
	//-Upper the results in case of a non-integer number (For resolutions such as 1360x768).
	unsigned int area_width = (unsigned int)ceil((float)_user_desktop._screen_resolusion.width / AREAS_PER_ROW);
	unsigned int area_height = (unsigned int)ceil((float)_user_desktop._screen_resolusion.height / AREAS_PER_ROW);

	//-Init each rect in the multidimensional array.
	for (unsigned int i = 0; i < AREAS_PER_ROW; ++i)
		for (unsigned int j = 0; j < AREAS_PER_ROW; ++j)
			_screen_gaze_areas[i][j] = Rect(j * area_width, i * area_height, area_width, area_height);
}

// Input: Name of the window.
// Output: Handle of the window.
//-Gets the handle(HWND) of the window.
HWND GazeTracker::getWindowHWND(const char* window_name) {
	return FindWindow(NULL, window_name);
}

// Output: Handle of the main window.
//-Initalize all the User detection windows.
HWND GazeTracker::initUserDetectionWindows() {
	//-Main camera window.
	namedWindow(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME, CV_WINDOW_NORMAL);
	resizeWindow(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME, _capture_device_resolusion.width, _capture_device_resolusion.height);
	moveWindow(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME, SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION.x, SETTINGS_CALIBRATION_MAIN_WINDOW_LOCATION.y);
	Desktop::setIcon(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME);

	//-Right eye pre-threshold window.
	namedWindow(RIGHT_EYE_PREVIEW_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	moveWindow(RIGHT_EYE_PREVIEW_WINDOW_NAME, RIGHT_EYE_PREVIEW_WINDOW_LOCATION.x, RIGHT_EYE_PREVIEW_WINDOW_LOCATION.y);
	removeWindowTitlebar(RIGHT_EYE_PREVIEW_WINDOW_NAME);

	//-Right eye post-threshold window.
	namedWindow(RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	moveWindow(RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME, RIGHT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION.x, RIGHT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION.y);
	removeWindowTitlebar(RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME);

	//-Left eye pre-threshold window.
	namedWindow(LEFT_EYE_PREVIEW_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	moveWindow(LEFT_EYE_PREVIEW_WINDOW_NAME, LEFT_EYE_PREVIEW_WINDOW_LOCATION.x, LEFT_EYE_PREVIEW_WINDOW_LOCATION.y);
	removeWindowTitlebar(LEFT_EYE_PREVIEW_WINDOW_NAME);

	//-Left eye post-threshold window.
	namedWindow(LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	moveWindow(LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME, LEFT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION.x, LEFT_EYE_THRESHED_PREVIEW_WINDOW_LOCATION.y);
	removeWindowTitlebar(LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME);

	//-Return the main window handle.
	return getWindowHWND(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME);
}

// Input: Name of the window.
// Output: Pointer of the window.
//-Initialize the window to fullscreen with no black borders.
HWND GazeTracker::initFullscreenWindow(const char* window_name) {
	//-Set window to full screen.
	namedWindow(window_name, CV_WINDOW_NORMAL);
	setWindowProperty(window_name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	
	//-Get the handle of the window.
	HWND window_hwnd = getWindowHWND(window_name);

	//-Delete window borders.
	SetClassLongPtr(window_hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0, 0, 0)));

	//-Return the handle of the window.
	return window_hwnd;
}

// Input: Name of the window.
//-Remove the titlebar of the window.
void GazeTracker::removeWindowTitlebar(const char* window_name) {
	//-Get the window handle.
	HWND hwnd = getWindowHWND(window_name);

	//-Change the style to be without borders and without a titlebar.
	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);

	//-Set the new style.
	SetWindowLong(hwnd, GWL_STYLE, lStyle);
}

// Input: Window of the current state of the program.
// Output: If window is alive or if user wished to close the program.
bool GazeTracker::keepRunning(HWND window){
	return IsWindowVisible(window) && _keep_alive;
}

// Input: Window of the current state of the program.
//-Close all windows and release the handle.
void GazeTracker::closeWindow(HWND window){
	SendMessage(window, WM_CLOSE, NULL, NULL);
	_keep_alive = true;
}

// Input: User's option(exit program or continue to the next step).
//-Init the return status code and the frame capturing loop condition.
void GazeTracker::proceed(ReturnCode option) {
	_return_status_code = option;
	_keep_alive = false;
}

//-Function used to let the user adjust the settings for face and eye detection. 
ReturnCode GazeTracker::calibrateDetectionSettings() {
	//-Get the main window handle.
	HWND settings_preview_window = initUserDetectionWindows();

	//-Open the controller window.
	_detection_controller.open();

	//-Start counting frames.
	FPSCounter FpsCounter;
	FpsCounter.start();

	while (keepRunning(settings_preview_window)){
		Mat frame = retriveFrame();

		//-Get user face and eye locations and values.
		UserIdentification current_user_identification = detectUserIdentification(frame, true, true);

		//-Perform 'Wink' test
		if (current_user_identification.userWinked(EyeSide::RIGHT))
			Desktop::MouseClickBeep();

		//-Get FPS value.
		string fps_text = FPS_INDICATION_TEXT + FpsCounter.getFpsString();

		//-Print FPS and available options.
		printText(frame, fps_text, FPS_INDICATION_TEXT_LOCATION);
		printText(frame, OPEN_CONTROLLER_TEXT, OPEN_CONTROLLER_TEXT_LOCATION);
		printText(frame, RESET_CONTROLLER_TEXT, RESET_CONTROLLER_TEXT_LOCATION);
		printText(frame, CONTINUE_TEXT, CONTINUE_TEXT_LOCATION);

		imshow(SETTINGS_CALIBRATION_MAIN_WINDOW_NAME, frame);

		switch (getKey()){
			case (RESET_CONTROLLER):
				_detection_controller.reset();
				break;
			case (OPEN_CONTROLLER):
				_detection_controller.open();
				break;
			case (MIRROR_FRAME):
				_mirror_frame= !_mirror_frame;
				break;
			case (QUIT):
				proceed(APPLICATION_EXIT);
				break;
			case (CONTINUE):
				proceed(GAZE_CALIBRATION);
				break;
		}
	}

	//-Close the controller window.
	_detection_controller.close();

	//-Close the windows.
	closeWindow(settings_preview_window);
	destroyAllWindows();

	return _return_status_code;
}

//-Function used to let the user calibrate the eye movment to mouse position ratio.
ReturnCode GazeTracker::calibrateRatio(){
	//-Initialize the window.
	HWND ratio_calibration_window = initFullscreenWindow(RATIO_CALIBRATION_MAIN_WINDOW_NAME);

	//-Set point size.
	const unsigned int calibration_point_size = 10;

	//-Set the screen calibration points colors.
	cvRGB primary_color = CV_BLUE;
	cvRGB secondery_color = CV_RED;

	//-Init array index.
	unsigned int points_array_index = 0;

	while (keepRunning(ratio_calibration_window)){
		Mat frame = retriveFrame();

		UserIdentification current_user_identification = detectUserIdentification(frame, true, false);
		if (!current_user_identification.foundEyes()) {
			displayNoDetectionWarning(frame, RATIO_CALIBRATION_MAIN_WINDOW_NAME);
			continue;
		}

		//-Add borders to the frames.
		addBorders(frame);

		//-Draw the calibration points.
		circle(frame, _base_screen_calibration_points[points_array_index], calibration_point_size, primary_color, FILLED);
		circle(frame, _base_screen_calibration_points[points_array_index], calibration_point_size / 2, secondery_color, FILLED);

		//-Print which eye will calibrate.
		printText(frame, CALIBRATED_EYE_TEXT, CALIBRATED_EYE_TEXT_LOCATION, CALIBRATED_EYE_SCALE, CV_BLACK);
		_eye_to_calibrate ? printText(frame, RIGHT_EYE_TEXT, CURRENT_CALIBRATED_EYE_TEXT_LOCATION, CALIBRATED_EYE_SCALE, CV_RED) : 
							printText(frame, LEFT_EYE_TEXT, CURRENT_CALIBRATED_EYE_TEXT_LOCATION, CALIBRATED_EYE_SCALE, CV_CYAN);

		//-Print switch calbrated eye notification.
		if (points_array_index == 0)
			printText(frame, CHANGE_CALIBRATED_EYE_TEXT, CHANGE_CALIBRATED_EYE_TEXT_LOCATION, DEFAULT_TEXT_SCALE, CV_BLACK);

		//-Print 'Go back' notification.
		printText(frame, GO_BACK_TEXT, GO_BACK_TEXT_LOCATION, DEFAULT_TEXT_SCALE, CV_BLACK);

		imshow(RATIO_CALIBRATION_MAIN_WINDOW_NAME, frame);

		switch (getKey()){
			case (MIRROR_FRAME):
				_mirror_frame= !_mirror_frame;
				break;
			case (SPACE):
				//-Only let the user change eye before the first calibration.
				if (points_array_index == 0)
					_eye_to_calibrate = (EyeSide)!_eye_to_calibrate;
				break;
			case (CONTINUE):
				//-Save gaze location.
				calibrate(current_user_identification, points_array_index);
				break;
			case (BACK):
				proceed(DETECTION_SETTINGS_CALIBRATION);
				break;
			case (QUIT):
				proceed(APPLICATION_EXIT);
				break;
		}

		swap(primary_color, secondery_color);
	}

	//calibrationReport();

	closeWindow(ratio_calibration_window);
	return _return_status_code;
}

//-Function for mouse controling.
ReturnCode GazeTracker::mouseControl(){
	//-Initialize the window.
	HWND screen_zoom_window = initFullscreenWindow(SCREEN_ZOOM_WINDOW_NAME);
	ShowWindow(screen_zoom_window, SW_MINIMIZE);

	//-Initialize and start the keyboard hook.
	KeyboardHook keyboard_hook(EXIT_COMBINATION_KEYS, &_keep_alive, false);
	keyboard_hook.track();

	//-Init custom cursor.
	_user_desktop.initCustomCursor();

	//-Init area selection GUI.
	_user_desktop.initSelectAreaGUI(_screen_gaze_areas[0][0]);

	bool is_zooming = false;

	Rect selected_area;
	Mat zoomed_area;

	while (keepRunning(screen_zoom_window)){
		Mat frame = retriveFrame();

		UserIdentification current_user_identification = detectUserIdentification(frame);
		if (!current_user_identification.foundEyes()) {
			displayNoDetectionWarning(frame, SCREEN_ZOOM_WINDOW_NAME);
			continue;
		}

		//-Get the area which the user is looking at.
		Point estimated_gaze = getEstimatedEyeGazeLocation(current_user_identification);
		Rect gaze_area = getGazeArea(estimated_gaze);

		//-Move the mouse cursor to that location.
		_user_desktop.moveCursor(gaze_area.tl(), getCursorLocation(gaze_area));

		//-If the user winked to zoom, display the zoomed area location.
		if (is_zooming) {
			imshow(SCREEN_ZOOM_WINDOW_NAME, zoomed_area);
			waitKey(1);
		}

		//-Check if the user winked.
		if (current_user_identification.userWinked(EyeSide::RIGHT)) {
			Desktop::MouseClickBeep();
			
			is_zooming = !is_zooming;

			//-Wink for entering zoom mode. else, exit the zoom mode.
			if (is_zooming) {
				_user_desktop.hideSelectAreaGUI();

				selected_area = gaze_area;
				zoomed_area = zoomScreenImage(selected_area);
				ShowWindow(screen_zoom_window, SW_MAXIMIZE);
			}
			else {
				_user_desktop.showSelectAreaGUI();
				ShowWindow(screen_zoom_window, SW_MINIMIZE);

				_user_desktop.moveCursor(gaze_area.tl(), getZoomedCursorLocation(selected_area, gaze_area));
				_user_desktop.cursorLeftClick();
			}
		}
	}

	_user_desktop.restoreDefaultCursor();
	_user_desktop.terminateSelectAreaGUI();

	closeWindow(screen_zoom_window);
	return APPLICATION_EXIT;
}

//-Function for testing mouse control while developing and testing head movement calibration.
void GazeTracker::mouseControlDevTest() {
	//-Initialize the window.
	HWND ratio_preview_window = initFullscreenWindow(MOUSE_TESTING_WINDOW_NAME);

	//-Initialize and start the keyboard hook.
	KeyboardHook keyboard_hook(EXIT_COMBINATION_KEYS, &_keep_alive, false);
	keyboard_hook.track();

	Rect selected_area;

	while (keepRunning(ratio_preview_window)) {
		Mat frame = retriveFrame();

		UserIdentification current_user_identification = detectUserIdentification(frame, true, false);
		if (!current_user_identification.foundEyes()) {
			displayNoDetectionWarning(frame, MOUSE_TESTING_WINDOW_NAME);
			continue;
		}

		addBorders(frame);

		circle(frame, _face_center_avg + _borders, 5, CV_RGB(0, 255, 0),-1);

		Point estimated_gaze = getEstimatedEyeGazeLocation(current_user_identification);

		Rect gaze_area = getGazeArea(estimated_gaze);
		Point area_center(gaze_area.x + gaze_area.width / 2, gaze_area.y + gaze_area.height / 2);

		rectangle(frame, gaze_area, CV_RGB(255, 0, 0));
		
		imshow(MOUSE_TESTING_WINDOW_NAME,frame);
		waitKey(1);
	}

	closeWindow(ratio_preview_window);
}

// Output: frame from the capture device.
//-Retriving frame from the capture device, mirror the image(unless the user told not to) for better tracking results.
//-Throws exception if the capture device disconnected.
Mat GazeTracker::retriveFrame(){
	Mat new_frame;

	//-Capture frame from the capture device.
	if (!_capture_device.retrieve(new_frame))
		throw exception(DEVICE_DISCONNECTED_ERROR);

	//-Mirror the image.
	if (_mirror_frame)
		flip(new_frame, new_frame, 1);

	return new_frame;
}

// Input: frame from the caputre device.
//-Adds white borders to the frame on each side.
void GazeTracker::addBorders(Mat &frame) {
	copyMakeBorder(frame, frame, _borders.second, _borders.second, _borders.first, _borders.first, BORDER_CONSTANT, CV_WHITE);
}

// Input: Frame from the capture device, text, location(Point).
// Optional Input: Color, scaling size.
//-Print text on a frame.
void GazeTracker::printText(Mat frame, string text, const Point location, double font_scale, cvRGB color) {
	putText(frame, text, location, FONT_HERSHEY_TRIPLEX, font_scale, color, 1, CV_AA);
}

// Input: Frame from the capture device, window name.
//-Display a frame with a warning about the detection of the eyes.
void GazeTracker::displayNoDetectionWarning(Mat frame, const char* window_name) {
	addBorders(frame);

	printText(frame, NO_EYE_WARNING_TEXT, NO_EYE_WARNING_TEXT_LOCATION, 1.5);

	imshow(window_name, frame);
	if (getKey() == QUIT)
		proceed(APPLICATION_EXIT);
}

// Input: Delay time between frames.
// Output: Value of the key pressed by the user.
//-Returns the lowercase value of the key. Delay time is set to 1 in order to get the minimal delay posibble.
char GazeTracker::getKey(unsigned int delay) {
	return tolower(waitKey(delay));
}

//======================================================================

UserIdentification pirior_identification;
// Input: Frame from the camera,init the user's eyes.
// Output: UserIdefntification values that stores the face and eyes position.
//-Detects user's eyes and face and draws their location on the frame.
UserIdentification GazeTracker::detectUserIdentification(Mat src, bool to_draw, bool show_process) {
	//-Change the src image to a gray image.
	Mat frame_gray;
	cvtColor(src, frame_gray, CV_BGR2GRAY);

	//-Find faces in the gray frame image.
	vector<Rect> faces = _simd_detector.getFaces(frame_gray);
	if (faces.empty())
		return UserIdentification();//-Return nothing found;

	//-Save only the first rect found.
	Rect user_face = faces[0];
	faces.clear();

	//-Check to see if it's probably the same user                                          
	if (abs(user_face.x - pirior_identification._face.x) < src.size().width / 6 && abs(user_face.y - pirior_identification._face.y) < src.size().height / 6)
		user_face = updatedRectPosition(user_face, pirior_identification._face, _detection_controller._face_position_sensetivity);

	//-Draw the face of the user.
	if (to_draw)
		rectangle(src, user_face, FACE_RECT_COLOR);

	//-Crop the src image to only keep the face.
	Mat faceROI = frame_gray(user_face);
	frame_gray.release();

	//-Detect eyes in the faceROI.
	vector<Rect> eyes = _simd_detector.getEyes(faceROI);
	if (eyes.empty())
		return UserIdentification(user_face);//-Return only face found.

	Eye right_eye, left_eye;
	for (unsigned int i = 0; i < eyes.size(); ++i) {
		//-Check if current rect is in a normal eye position.
		if (user_face.y + eyes[i].y > user_face.y + user_face.width / 2)
			continue;

		//-Get side of eye and check if the eye was already detected.
		EyeSide eye_side = getSideOfEye(user_face, eyes[i]);//-Holds true if right,false if left.
		if (eye_side == EyeSide::RIGHT && !right_eye.isEmpty() || eye_side == EyeSide::LEFT && !left_eye.isEmpty())
			continue;

		Rect current_eye = eye_side ? updatedRectPosition(eyes[i], pirior_identification._rightEye._eye_rect, _detection_controller._eyes_position_sensetivity) :
									  updatedRectPosition(eyes[i], pirior_identification._leftEye._eye_rect, _detection_controller._eyes_position_sensetivity);

		//-Crop the eye image from the face image and add brightness.
		Mat eyeROI = faceROI(current_eye).clone() + Scalar(_detection_controller._brightness, _detection_controller._brightness, _detection_controller._brightness);

		//-Change contrast.
		eyeROI.convertTo(eyeROI, -1, (double)_detection_controller._contrast / 10);

		//-Blur the ROI.
		blurImage(eyeROI);

		//-Show pre-threshold image.
		if (show_process)
			eye_side ? imshow(RIGHT_EYE_PREVIEW_WINDOW_NAME, eyeROI) : imshow(LEFT_EYE_PREVIEW_WINDOW_NAME, eyeROI);

		//-Threshold the eye image to a binary image.
		threshold(eyeROI, eyeROI, _detection_controller._threshold, DetectionControllerConstants::THRESHOLD_MAX, CV_THRESH_BINARY_INV);

		removeTopRows(eyeROI, (unsigned int)(eyeROI.rows / (10 - _detection_controller._eyebrow_remove)));

		//-Dilate(fill) the contours.
		const unsigned int dilation_size = 1;
		Mat dilation_element = getStructuringElement(0,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
		///-Apply the dilation operation
		dilate(eyeROI, eyeROI, dilation_element);

		if (show_process)
			eye_side ? imshow(RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME, eyeROI) : imshow(LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME, eyeROI);

		//-Find contours in the thresholded image.
		vector<vector<Point>> contours;
		findContours(eyeROI.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		drawContours(eyeROI, contours, -1, CV_WHITE, 3);

		double max_area = 0;
		Rect pupil_rect;
		unsigned int pupil_radius_X;
		unsigned int pupil_radius_Y;

		//-Find the biggest rect.
		for (unsigned int j = 0; j < contours.size(); ++j) {
			double area = contourArea(contours[j]);

			if (area > max_area) {
				max_area = area;
				pupil_rect = boundingRect(contours[j]);
				pupil_radius_X = (unsigned int)(pupil_rect.width / 2);
				pupil_radius_Y = (unsigned int)(pupil_rect.height / 2);
			}
		}

		//-If a rect have been found, update eye location.
		if (max_area != 0) {
			Point eye_pupil(user_face.x + current_eye.x + pupil_rect.x + pupil_radius_X, user_face.y + current_eye.y + pupil_rect.y + pupil_radius_Y);
			Point eye_center(user_face.x + current_eye.x + current_eye.width / 2, user_face.y + current_eye.y + current_eye.height / 2);
			Point eye_corner = eye_side ? Point(user_face.x + current_eye.x, user_face.y + current_eye.y + current_eye.height / 2 + 2) : Point(user_face.x + current_eye.x + current_eye.width, user_face.y + current_eye.y + current_eye.height / 2 + 2);
			int eye_radius = cvRound((current_eye.width + current_eye.height) * 0.25);

			eye_side ? right_eye = Eye(current_eye, pupil_rect, eye_pupil, eye_center, eye_corner, eye_radius, eye_side) : left_eye = Eye(current_eye, pupil_rect, eye_pupil, eye_center, eye_corner, eye_radius, eye_side);
		}

	}

	//-Clear windows if an eye wasn't found.
	if (right_eye.isEmpty() || left_eye.isEmpty() && show_process) {
		if (right_eye.isEmpty()) {
			imshow(RIGHT_EYE_PREVIEW_WINDOW_NAME, Media::_no_eye_image);
			imshow(RIGHT_EYE_THRESHED_PREVIEW_WINDOW_NAME, Media::_no_eye_image);
		}
		if (left_eye.isEmpty()) {
			imshow(LEFT_EYE_PREVIEW_WINDOW_NAME, Media::_no_eye_image);
			imshow(LEFT_EYE_THRESHED_PREVIEW_WINDOW_NAME, Media::_no_eye_image);
		}
	}

	//-Draw the pupils location.
	if (to_draw) {
		right_eye.draw(src);
		left_eye.draw(src);
	}

	//-Save current identification values.
	UserIdentification current_identification(user_face, right_eye, left_eye);
	pirior_identification = current_identification;

	return current_identification;//-Return both face and eyes found.
}

// Input: Current rect, pirior rect and sensetivity.
// Output: Most precise location of the rect dependet on the user prefernece.
//-Check to see if the user moved enough to update position of the rect.                           
Rect GazeTracker::updatedRectPosition(Rect current_rect, Rect pirior_rect, int rect_sensetivity){
	return (abs(current_rect.x - pirior_rect.x) < rect_sensetivity && abs(current_rect.y - pirior_rect.y) < rect_sensetivity) ? pirior_rect : current_rect;
}

// Input: Face of the user and one of his eyes.
// Output: Return true(EyeSide::RIGHT) if is right eye,false(EyeSide::LEFT) if left.
//-Function used to get the side of the eye on the face of the user.
EyeSide GazeTracker::getSideOfEye(Rect face, Rect eye) {
	return (EyeSide)((float)eye.x <= (face.width / 2));
}

// Input: ROI of the eye.
//-Blur the eye image to get rid of unnecessary noises(eye lashes,etc..) 
void GazeTracker::blurImage(Mat& eyeROI) {
	unsigned int kernel_size_X = _detection_controller._blur_ksize_X;
	unsigned int kernel_size_Y = _detection_controller._blur_ksize_Y;

	//-Since the OpenCV blur function only accept odd integers, we add 1 to the integer to make sure its an odd number.
	if (kernel_size_X % 2 == 0 || kernel_size_X == 0)
		++kernel_size_X;

	if (kernel_size_Y % 2 == 0 || kernel_size_Y == 0)
		++kernel_size_Y;

	//-After correcting the controller values, we can now apply the blur to the ROI.
	blur(eyeROI, eyeROI, Size(kernel_size_X, kernel_size_Y));
}

// Input: frame from camera,number of rows to remove.
//-Removes top layers of the mat to hide user's eyebrows.
void GazeTracker::removeTopRows(Mat src, unsigned int number_of_rows) {
	for (unsigned int y = 0; y < number_of_rows; ++y) {
		//-Get pointer of the row.
		float* current_row_ptr = src.ptr<float>(y);
		for (unsigned int x = 0; x < (unsigned int)src.cols; ++x)
			current_row_ptr[x] = 0.; //-Paint pixel value to black (using float values).
	}
}

//======================================================================

// Output: Return the average point of the user point of view.
//-Capture frames X amount of times and return the avarage point of the user point of view.
Point GazeTracker::getEyePovAvgPoint() {
	Point eye_avg_point;

	unsigned int successful_iterations = 0;

	for (unsigned int iteration = 0; iteration < AVG_ITERATION_NUMBER; ++iteration) {
		Mat frame = retriveFrame();

		UserIdentification current_user_identification = detectUserIdentification(frame, false, false);
		if (!current_user_identification.foundEyes())
			continue;

		eye_avg_point += _eye_to_calibrate ? current_user_identification._rightEye._pupil : current_user_identification._leftEye._pupil;
		++successful_iterations;

	}

	return (eye_avg_point / successful_iterations);
}

// Input: User identification, index of the calibration points array. 
//-Save the avarge eye pov in the array, calculate the avarage face position. Once the array is full the function will update the caller function frame capture loop.
//-The function auto increase the array index.
void GazeTracker::calibrate(UserIdentification current_user_identification, unsigned int& index) {
	//-Save avarage eye POV.
	_user_calibration_points[index] = getEyePovAvgPoint();

	//-Get the current face center value.
	_face_center_avg += current_user_identification.getFaceCenter();

	++index;
	if (index == NUMBER_OF_CALIBRATION_POINTS) {
		//-Avarage the face center position.
		_face_center_avg /= NUMBER_OF_CALIBRATION_POINTS;

		proceed(MOUSE_CONTROL);
	}
}

//-For future developing.
void GazeTracker::calibrationReport() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	string str("b");
	str += ".txt";

	ofstream myfile(str);

	myfile << "Face Size: " << _face_center_avg << "\n\n";
	myfile << "Face Center: " << _face_center_avg << "\n\n";

	myfile << "TOP: " << _user_calibration_points[TOP] << "\n";
	myfile << "LEFT: " << _user_calibration_points[LEFT] << "\n";
	myfile << "MIDDLE: " << _user_calibration_points[MIDDLE] << "\n";
	myfile << "RIGHT: " << _user_calibration_points[RIGHT] << "\n";
	myfile << "BOTTOM: " << _user_calibration_points[BOTTOM];

	myfile.close();
}

//======================================================================

//!Currently, the function won't do anything, but we are working on it.
// Input: Current user face center.
// Output: Fixed calibration points.
//-Fix the calibration points for head movement.
GazeTracker::CalibrationPoints GazeTracker::getFixedCalibrationPoints(Point user_face_center) {
	//-Copy the calibrated user points.
	CalibrationPoints adjusted_points = _user_calibration_points;

	int subx = user_face_center.x - _face_center_avg.x;
	int suby = user_face_center.y - _face_center_avg.y;



	return adjusted_points;
}

// Input: Fixed calibration points, current pupil location, side, axis.
// Output: Estimated axis value of the current gaze.
//-Calculate the estimated eye gaze axis based on the calibration points.
unsigned int GazeTracker::getAxisValue(CalibrationPoints point_array, Point pupil_location ,unsigned int side, bool axis) {
	int n = (side == TOP || side == LEFT) ? -1 : 1;

	int a = axis ? point_array[MIDDLE].x - pupil_location.x : point_array[MIDDLE].y - pupil_location.y;
	int b = axis ? point_array[MIDDLE].x - point_array[side].x : point_array[MIDDLE].y - point_array[side].y;

	float c = 1 + n * ((float)a / (float)b);

	return (unsigned int) (axis ? _user_desktop._screen_center.x * c : _user_desktop._screen_center.y * c);
}

Point pirior_estimated_gaze;
// Input: Current user identification.
// Output: Estimated gaze point.
//-Calculate the estimated eye gaze.
Point GazeTracker::getEstimatedEyeGazeLocation(UserIdentification current_user_identification) {
	Point current_eye_pupil = _eye_to_calibrate ? current_user_identification._rightEye._pupil : current_user_identification._leftEye._pupil;
	CalibrationPoints fixed_calibration_points = getFixedCalibrationPoints(current_user_identification.getFaceCenter());

	//-Get current gaze location.
	Point current_estimated_gaze;
	current_estimated_gaze.x = (current_eye_pupil.x < fixed_calibration_points[MIDDLE].x) ? getAxisValue(fixed_calibration_points, current_eye_pupil, LEFT, true) : getAxisValue(fixed_calibration_points, current_eye_pupil, RIGHT, true);
	current_estimated_gaze.y = (current_eye_pupil.y < fixed_calibration_points[MIDDLE].y) ? getAxisValue(fixed_calibration_points, current_eye_pupil, TOP, false) : getAxisValue(fixed_calibration_points, current_eye_pupil, BOTTOM, false);

	//-Smooth location.
	current_estimated_gaze = (pirior_estimated_gaze + current_estimated_gaze) / 2;

	//-If current location is beyond screen size, change it to 1 pixel lower than the screen resolution.
	if (current_estimated_gaze.x > _user_desktop._screen_resolusion.width)
		current_estimated_gaze.x = _user_desktop._screen_resolusion.width - 1;

	if (current_estimated_gaze.y > _user_desktop._screen_resolusion.height)
		current_estimated_gaze.y = _user_desktop._screen_resolusion.height - 1;

	pirior_estimated_gaze = current_estimated_gaze;

	return current_estimated_gaze;
}

Rect pirior_gaze_area;
// Input: Estimated gaze point.
// Output: Gaze area.
//-Find the estimated gaze area.
Rect GazeTracker::getGazeArea(Point estimated_eye_gaze) {
	for (unsigned int i = 0; i < AREAS_PER_ROW; ++i)
		for (unsigned int j = 0; j < AREAS_PER_ROW; ++j)
			if (_screen_gaze_areas[i][j].contains(estimated_eye_gaze)) {
				pirior_gaze_area = _screen_gaze_areas[i][j];
				return _screen_gaze_areas[i][j];
			}

	//-If the estimated eye gaze wan't found in any area, return the pirior one.
	return pirior_gaze_area;
}

// Input: Selected area.
// Output: Zoomed image of the screen area.
//-Get image of the screen and zoom on the slected area.
Mat GazeTracker::zoomScreenImage(Rect area) {
	//-Get image of the screen.
	Mat screen_image = _user_desktop.getDesktopImage();

	//-Create zoom effect.
	screen_image = screen_image(area);
	resize(screen_image, screen_image, _user_desktop._screen_resolusion);

	return screen_image;
}

// Input: Gaze area.
// Output: Cursor location.
//-Returns the cursor location based on the gaze area.
Point GazeTracker::getCursorLocation(Rect gaze_area) {
	return gaze_area.tl() + (gaze_area.size() / 2);
}

// Input: Outer gaze area, Zoomed gaze area.
// Output: Cursor location.
//-Returns the cursor location based on the outer gaze area(pre-wink) and the zoomed gaze area(post-wink).
Point GazeTracker::getZoomedCursorLocation(Rect outer, Rect zoomed) {
	return outer.tl() + (zoomed.tl() / AREAS_PER_ROW);
}