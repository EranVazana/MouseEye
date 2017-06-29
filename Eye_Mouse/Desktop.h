#pragma once
#define OEMRESOURCE
#include <Windows.h>

#include <opencv2\opencv.hpp>

#include "Media.h"

#include "ZoomAreaGUI.h"

class ZoomAreaGUI;

using namespace cv;

typedef Size Resolution;

//-Class used for working with the user desktop.
class Desktop {
public:
	Desktop();
	~Desktop();

	Mat getDesktopImage();

	void moveCursor(Point, Point);
	void cursorLeftClick();

	void initSelectAreaGUI(Rect, bool = true);
	void hideSelectAreaGUI();
	void showSelectAreaGUI();
	void terminateSelectAreaGUI();

	void initCustomCursor();
	void restoreDefaultCursor();

	static HWND GetWindowHWND(const char*);
	static void SetIcon(const char*);
	static void SetIcon(HWND);
	static void MouseClickBeep();
	static void ErrorSound();

	Resolution _screen_resolusion;
	Point _screen_center;

private:
	#define CURSOR_X_MULTIPLIER (65536.0f / GetSystemMetrics(SM_CXSCREEN))
	#define CURSOR_Y_MULTIPLIER (65536.0f / GetSystemMetrics(SM_CYSCREEN))

	#define CUSTOM_CURSOR_FILENAME "Cursor/Selection_Cursor.cur"

	static void PerformBeep();

	BITMAPINFOHEADER  _bi;

	Point _cursor_location;
	ZoomAreaGUI* _zoom_area_GUI;

	HCURSOR _default_arrow_cursor;
};