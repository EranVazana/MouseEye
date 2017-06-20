#pragma once
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

//-Class used to measure the frames per second values when detecting the user's face values.
class FPSCounter {
public:
	FPSCounter();
	~FPSCounter();

	void start();
	unsigned int getFPS();
	string getFpsString();

private:
	bool toReset();

	time_t _start, _end;
	__int64 _frames_counter;
};