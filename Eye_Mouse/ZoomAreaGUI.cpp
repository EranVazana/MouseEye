#include "ZoomAreaGUI.h"

//======================================================================

//-Constructor.
ZoomAreaGUI::ZoomAreaGUI(Rect area_values) {
	_window_process = nullptr;
	_window_hwnd = nullptr;
	_area_values = area_values;
}

//-Destructor.
ZoomAreaGUI::~ZoomAreaGUI() {
	TerminateProcess(_window_process, 1);
}

// Input: Area values.
//-Opens the GUI and initalize the area values.
void ZoomAreaGUI::activate() {
	//-Init the shell values.
	SHELLEXECUTEINFO shellExInfo;

	shellExInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellExInfo.hwnd = NULL;
	shellExInfo.lpVerb = "open";
	shellExInfo.lpFile = GUI_JAR_FILENAME;
	shellExInfo.lpParameters = getGUIParameters();
	shellExInfo.lpDirectory = NULL;
	shellExInfo.nShow = SW_SHOW;
	shellExInfo.hInstApp = NULL;

	//-Open the GUI, Throw exception if an error occoured.
	if (!ShellExecuteEx(&shellExInfo))
		throw exception(ZOOM_AREA_GUI_SETUP_ERROR);

	GetProcessId(shellExInfo.hProcess);
	_window_process = shellExInfo.hProcess;

	WaitForInputIdle(_window_process, INFINITE);

	//-Wait for window to open.
	do {
		_window_hwnd = FindWindow(NULL, ZOOM_AREA_GUI_WINDOW_NAME);
		this_thread::sleep_for(chrono::milliseconds(20));
	} while (!IsWindowVisible(_window_hwnd));
}

//-Hide the GUI icon from the taskbar.
void ZoomAreaGUI::hideTaskbarIcon() {
	ITaskbarList* pTaskList = NULL;
	HRESULT initRet = CoInitialize(NULL);
	HRESULT createRet = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (LPVOID*)&pTaskList);

	if (createRet == S_OK) {
		pTaskList->DeleteTab(_window_hwnd);
		pTaskList->Release();
	}

	CoUninitialize();
}

//-Hide the GUI.
void ZoomAreaGUI::hide() {
	ShowWindow(_window_hwnd, SW_MINIMIZE);
}

//-Show the GUI.
void ZoomAreaGUI::show() {
	ShowWindow(_window_hwnd, SW_MAXIMIZE);
}

// Input: New area position.
//-Move the GUI to the new location.
void ZoomAreaGUI::moveArea(Point new_area_location) {
	MoveWindow(_window_hwnd, new_area_location.x, new_area_location.y, _area_values.width, _area_values.height, false);
}

// Input: Area values.
// Output: GUI Jar's parameters.
//-Converts all the Jar's parameters to one string.
const char* ZoomAreaGUI::getGUIParameters() {
	//-Init the GUI parameters.
	string parameters = to_string(_area_values.width) + " " + to_string(_area_values.height) + " " + to_string(GUI_BORDER_THICKNESS) + " \"" + GUI_BORDER_COLOR + "\"" + " \"" + ZOOM_AREA_GUI_WINDOW_NAME + "\"";
	return parameters.c_str();
}