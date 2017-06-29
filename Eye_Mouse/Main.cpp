#include "GazeTracker.h"
#include "ModifiedConsole.h"

//-Main application function.
int main(const unsigned int argc, const char** argv) {
	ModifiedConsole::DestroyDefaultConsole();

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
	catch (const exception& exception_from_GazeTracker){
		ModifiedConsole* _modifed_console = new ModifiedConsole();

		Desktop::ErrorSound();
		_modifed_console->Print(exception_from_GazeTracker.what());

		this_thread::sleep_for(chrono::milliseconds(1500));
		_modifed_console->WaitForKeyPress();

		return -1;
	}

	return 0;
}