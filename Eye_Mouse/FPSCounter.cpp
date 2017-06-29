#include "FPSCounter.h"

//-Constructor.
FPSCounter::FPSCounter() {
	_frames_counter = 0;
}

//-Destructor.
FPSCounter::~FPSCounter() {

}

//-Start calculating the frame rate.
void FPSCounter::start() {
	time(&_start);
}

// Output: number of frames passed since began counting.
//-Calculates the number of frames that were proccesed in a second.
unsigned int FPSCounter::getFPS() {
	time(&_end);
	++_frames_counter;

	double sec = difftime(_end, _start);
	double fps = _frames_counter / sec;

	return (unsigned int)fps;
}

// Output: Text object of the fps value.
Text FPSCounter::getFpsText() {
	string content = TEXT_INTRO + to_string(getFPS());

	return Text(content, TEXT_LOCATION);
}