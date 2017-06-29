#include "Text.h"

//-Full constructor.
Text::Text(string content, Point location, double scale, cvRGB color) {
	_content = content;
	_location = location;
	_scale = scale;
	_color = color;
}

//-Defualt color constructor.
Text::Text(string content, Point location, double scale) {
	_content = content;
	_location = location;
	_scale = scale;
	_color = DEFAULT_TEXT_COLOR;
}

//-Defualt color and scale constructor.
Text::Text(string content, Point location) {
	_content = content;
	_location = location;
	_scale = DEFAULT_TEXT_SCALE;
	_color = DEFAULT_TEXT_COLOR;
}

//-Destructor.
Text::~Text() {

}