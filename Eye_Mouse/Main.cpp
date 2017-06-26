#include "GazeTracker.h"
#include "Console.h"

//-Main application function.
int main(unsigned int argc, const char** argv) {
	Console::HideConsole();

	try {
		GazeTracker gaze_tracker;
		ReturnCode return_code;

		return_code = gaze_tracker.calibrateDetectionSettings();
		while (return_code != APPLICATION_EXIT)
			switch (return_code) {
				case (DETECTION_SETTINGS_CALIBRATION):
					return_code = gaze_tracker.calibrateDetectionSettings();
					break;
				case (GAZE_CALIBRATION):
					return_code = gaze_tracker.calibrateGazeRatio();
					break;
				case (MOUSE_CONTROL):
					return_code = gaze_tracker.mouseControl();
					break;
			}
	}
	catch (const exception& exception_from_gazeTracker){
		Console::ShowConsole();
		Console::HideConsoleCursor();

		Desktop::ErrorSound();
		Console::Print(exception_from_gazeTracker.what());

		this_thread::sleep_for(chrono::milliseconds(1500));
		Console::WaitForKeyPress();

		return -1;
	}

	return 0;
}