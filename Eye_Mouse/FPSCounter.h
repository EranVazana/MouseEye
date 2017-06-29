#pragma once
#include <iostream>
#include <string>
#include <time.h>

#include "Text.h"

using namespace std;

//-Class used to measure the frames per second values when detecting the user's face values.
class FPSCounter {
public:
	FPSCounter();
	~FPSCounter();

	void start();
	unsigned int getFPS();
	Text getFpsText();

private:
	#define TEXT_INTRO "FPS: "
	#define TEXT_LOCATION Point(5,15)

	time_t _start, _end;
	unsigned __int64 _frames_counter;
};