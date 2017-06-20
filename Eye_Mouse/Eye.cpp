#include "Eye.h"

//======================================================================

//-Constructor.
Eye::Eye(Rect eye_rect, Rect pupil_rect, Point pupil,Point eye_center, Point corner, int eye_radius, EyeSide side) {
	_eye_rect = eye_rect;
	_pupil_rect = pupil_rect;
	_pupil = pupil;
	_eye_center = eye_center;
	_corner = corner;
	_eye_radius = eye_radius;
	_side = side;
}

//-Empty eye Constructor.
Eye::Eye() {
	_eye_rect = Rect();
	_pupil = Point(-1, -1);
	_eye_center = Point(-1, -1);
	_corner = Point(-1, -1);
	_eye_radius = -1;
	_side = EyeSide::LEFT;
}

//-Copy Constructor.
Eye::Eye(const Eye &other){
	_eye_rect = other._eye_rect;
	_pupil = other._pupil;
	_eye_center = other._eye_center;
	_corner = other._corner;
	_side = other._side;
}

//-Destructor.
Eye::~Eye() {

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
	s += "Eye Radius: " + std::to_string(_eye_radius) + "\n";
	s += "Pupil Size: " + std::to_string(_pupil_rect.width) + "," + std::to_string(_pupil_rect.height) + "\n";
	return s;
}