#pragma once
#include <Windows.h>
#include <thread>
#include <ShObjIdl.h>

#include <opencv2\opencv.hpp>

#include "Windows_Settings.h"
#include "Errors_And_Warnings.h"

using namespace std;
using namespace cv;

class ZoomAreaGUI {
public:
	ZoomAreaGUI(Rect);
	~ZoomAreaGUI();

	void activate();
	void hideTaskbarIcon();

	void hide();
	void show();

	void moveArea(Point);

private:
	#define GUI_JAR_FILENAME "Cursor\\ZoomAreaGUI.jar"
	#define GUI_BORDER_THICKNESS 5
	#define GUI_BORDER_COLOR "rgb(255, 0, 0)"

	const char* getGUIParameters();

	HANDLE _window_process;
	HWND _window_hwnd;

	Rect _area_values;
};