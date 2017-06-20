#include "GazeTracker.h"
#include "Console.h"

//-Main application function.
int main(unsigned int argc, const char** argv) {
	Console::HideConsole();

	try {
		GazeTracker gazeTracker;
		ReturnCode return_code;

		return_code = gazeTracker.calibrateDetectionSettings();
		while (return_code != APPLICATION_EXIT)
			switch (return_code) {
				case (DETECTION_SETTINGS_CALIBRATION):
					return_code = gazeTracker.calibrateDetectionSettings();
					break;
				case (GAZE_CALIBRATION):
					return_code = gazeTracker.calibrateRatio();
					break;
				case (MOUSE_CONTROL):
					return_code = gazeTracker.mouseControl();
					break;
			}
	}
	catch (exception exception_from_gazeTracker){
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