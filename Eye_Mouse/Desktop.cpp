#include "Desktop.h"

//======================================================================

//-Constructor.
Desktop::Desktop() {
	//-Get screen resolution values.
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	_screen_resolusion = Resolution(screen_width, screen_height);
	_screen_center = Point(screen_width / 2, screen_height / 2);

	//-Init bitmap settings.
	_bi.biSize = sizeof(BITMAPINFOHEADER);
	_bi.biWidth = screen_width;
	_bi.biHeight = -screen_height;
	_bi.biPlanes = 1;
	_bi.biBitCount = 32;
	_bi.biCompression = BI_RGB;
	_bi.biSizeImage = 0;
	_bi.biXPelsPerMeter = 0;
	_bi.biYPelsPerMeter = 0;
	_bi.biClrUsed = 0;
	_bi.biClrImportant = 0;

	_default_arrow_cursor = nullptr;
	_zoom_area_GUI = nullptr;
}

//-Destructor.
Desktop::~Desktop() {
	restoreDefaultCursor();
	terminateSelectAreaGUI();
}

// Output: Returns image of the desktop.
//-Get image of the desktop and convert it to Mat.
Mat Desktop::getDesktopImage() {
	HWND hwnd = GetDesktopWindow();

	//-Get the screen values.
	HDC hwindowDC = GetDC(hwnd);
	HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	//-Create a bitmap
	HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, _screen_resolusion.width, _screen_resolusion.height);

	//-Create desired Mat.
	Mat image(_screen_resolusion.height, _screen_resolusion.width, CV_8UC4);

	//-Use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	//-Copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, _screen_resolusion.width, _screen_resolusion.height, hwindowDC, 0, 0, _screen_resolusion.width, _screen_resolusion.height, SRCCOPY); 
	//-Copy from hwindowCompatibleDC to hbwindow
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, _screen_resolusion.height, image.data, (BITMAPINFO *)&_bi, DIB_RGB_COLORS);

	//-Avoid memory leak																								   
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return image;
}

// Input: Selection area values.
// Optional: Option to hide the GUI taskbar icon.
//-Opens the Zoom Area Selection GUI and hides the GUI icon from the taskbar if specified.
void Desktop::initSelectAreaGUI(Rect area, bool hide_taskbar_icon) {
	_zoom_area_GUI = new ZoomAreaGUI(area);
	_zoom_area_GUI->activate();

	if (hide_taskbar_icon)
		_zoom_area_GUI->hideTaskbarIcon();
}

//-Hides the Zoom Area Selection GUI.
void Desktop::hideSelectAreaGUI() {
	_zoom_area_GUI->hide();
}

//-Shows the Zoom Area Selection GUI.
void Desktop::showSelectAreaGUI() {
	_zoom_area_GUI->show();
}

//-Close the Zoom Area Selection GUI.
void Desktop::terminateSelectAreaGUI() {
	if (_zoom_area_GUI != nullptr) {
		delete _zoom_area_GUI;
		_zoom_area_GUI = nullptr;
	}
}

//-Initalize the custom cursor
void Desktop::initCustomCursor() {
	//-Save the default cursor.
	_default_arrow_cursor = CopyCursor(LoadCursor(0, IDC_ARROW));

	//-Set the new cursor.
	SetSystemCursor(LoadCursorFromFile(CUSTOM_CURSOR_FILENAME), OCR_NORMAL);
}

//-Change the cursor to the OS default cursor.
void Desktop::restoreDefaultCursor() {
	SetSystemCursor(_default_arrow_cursor, OCR_NORMAL);
}

//-Input: New cursor location.
//-Move the cursor to the new location.
void Desktop::moveCursor(Point new_area_location, Point new_cursor_location) {
	_zoom_area_GUI->moveArea(new_area_location);

	_cursor_location = new_cursor_location;
	SetCursorPos(_cursor_location.x, _cursor_location.y);
}

//-Perform a left click action.
void Desktop::cursorLeftClick() {
	//-Init input values.
	INPUT Inputs[3] = { NULL };

	//-Move the pointer location.
	Inputs[0].type = (LONG)INPUT_MOUSE;
	Inputs[0].mi.dx = (LONG)((_cursor_location.x - 1) * CURSOR_X_MULTIPLIER); //-X Coordinate.
	Inputs[0].mi.dy = (LONG)((_cursor_location.y - 1) * CURSOR_Y_MULTIPLIER); //-Y Coordinate.
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	//-Click on the location.
	Inputs[1].type = INPUT_MOUSE;
	Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	//-'Release' the click.
	Inputs[2].type = INPUT_MOUSE;
	Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	//-Send the input information to the OS.
	SendInput(3, Inputs, sizeof(INPUT));
}

//-Make a beep sound.
void Desktop::MouseClickBeep() {
	//-Call in a new thread for little to no drops in the frame rate.
	thread beep_thread(Desktop::performBeep);
	beep_thread.detach();
}

// Input: Name of the window.
//-Set the window icon to the program icon.
void Desktop::setIcon(const char* window_name) {
	SendMessage(FindWindow(NULL,window_name), WM_SETICON, ICON_SMALL, (LPARAM)Media::_program_icon);
}

//-Make an error sound.
void Desktop::ErrorSound() {
	MessageBeep(MB_ICONERROR);
}

//-Perform the beep sound.
void Desktop::performBeep() {
	Beep(1500, 100);
}