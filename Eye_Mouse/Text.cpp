#include "Text.h"

Text::Text(string content, Point location, double scale, cvRGB color) {
	_content = content;
	_location = location;
	_scale = scale;
	_color = color;
}

Text::Text(string content, Point location, double scale) {
	_content = content;
	_location = location;
	_scale = scale;
	_color = DEFAULT_TEXT_COLOR;
}

Text::Text(string content, Point location) {
	_content = content;
	_location = location;
	_scale = DEFAULT_TEXT_SCALE;
	_color = DEFAULT_TEXT_COLOR;
}

Text::~Text() {

}