#include "FPSCounter.h"

//-Constructor.
FPSCounter::FPSCounter() {
	_frames_counter = 0;
}

//-Destructor.
FPSCounter::~FPSCounter() {

}

//-Start calculating frames per seconds.
void FPSCounter::start() {
	time(&_start);
}

// Output: number of frames passed since began counting.
//-Calculates the number of frames that were proccesed in a second.
unsigned int FPSCounter::getFPS() {
	time(&_end);
	++_frames_counter;

	if (toReset()) {
		time(&_start);
		_frames_counter = 0;
	}

	double sec = difftime(_end, _start);
	double fps = _frames_counter / sec;

	return (unsigned int)fps;
}

// Output: number of frames passed since began counting as a string value.
//-ToString function for getFPS.
string FPSCounter::getFpsString() {
	return to_string(getFPS());
}

// Output: True if needed to reset the frame counter, False otherwise.
//-Checks if the counter reached its maximum value.
bool FPSCounter::toReset() {
	return _frames_counter == INT64_MAX - 1;
}