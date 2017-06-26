#pragma once
#include <string>

#include "Colors.h"

using namespace std;

//-Class used to define text values.
class Text {
public:
	#define DEFAULT_TEXT_SCALE 0.5
	#define DEFAULT_TEXT_COLOR CV_GREEN

	#define TEXT_FONT FONT_HERSHEY_TRIPLEX
	#define TEXT_THICKNESS 1
	#define TEXT_LINETYPE CV_AA

	Text(string, Point, double, cvRGB);
	Text(string, Point, double);
	Text(string, Point);
	~Text();

	string _content;
	Point _location;
	double _scale;
	cvRGB _color;

private:
	
};