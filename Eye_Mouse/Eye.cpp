#include "Eye.h"

//======================================================================

//-Constructor.
Eye::Eye(Rect eye_rect, Point pupil, EyeSide side) {
	_eye_rect = eye_rect;
	_pupil = pupil;
	_side = side;
}

//-Empty eye Constructor.
Eye::Eye() {
	_eye_rect = Rect();
	_pupil = Point(-1, -1);
	_side = EyeSide::LEFT;
}

//-Copy Constructor.
Eye::Eye(const Eye &other){
	_eye_rect = other._eye_rect;
	_pupil = other._pupil;
	_side = other._side;
}

//-Destructor.
Eye::~Eye() {

}

//-Returns the radius of the eye.
unsigned int Eye::getEyeRadius() {
	return cvRound((_eye_rect.width + _eye_rect.height) * 0.25);
}

// Input: Frame from camera.
//-Draws Eye position on the image.
void Eye::draw(Mat src){
	cvRGB color = _side ? RIGHT_EYE_MARK : LEFT_EYE_MARK;
	circle(src, _pupil, PUPIL_RADIUS, color, PUPIL_THICKNESS);
}

// Output: Return true is eye is empty(Not found while searching).
bool Eye::isEmpty(){
	return _pupil == Point(-1,-1);
}

//!-For debuging.
//-To string function.
Eye::operator std::string(){
	std::string s = "Eye Size: " + std::to_string(_eye_rect.width) + "," + std::to_string(_eye_rect.height) + "\n";
	s += "Eye Radius: " + std::to_string(getEyeRadius()) + "\n";
	return s;
}