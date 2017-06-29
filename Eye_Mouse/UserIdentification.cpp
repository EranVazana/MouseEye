#include "UserIdentification.h"

//======================================================================

//-Only face found Constructor.
UserIdentification::UserIdentification(Rect face) {
	_face = face;
	_rightEye = Eye();
	_leftEye = Eye();
}

//-Both face and eyes found Constructor.
UserIdentification::UserIdentification(Rect face, Eye rightEye, Eye leftEye) {
	_face = face;
	_rightEye = rightEye;
	_leftEye = leftEye;
}

//-No face and no eyes found Constructor.
UserIdentification::UserIdentification() {
	_face = Rect();
	_rightEye = Eye();
	_leftEye = Eye();
}

//-Copy constuctor.
UserIdentification::UserIdentification(const UserIdentification & other) {
	_face = other._face;
	_rightEye = other._rightEye;
	_leftEye = other._leftEye;
}

//-Destructor.
UserIdentification::~UserIdentification(){

}

// Output: Return true if a face was found.
//-Check if the object contain values of the face.
bool UserIdentification::foundFace(){
	return _face.width != 0;
}

// Output: Return true if eyes were found.
//-Check if the object contain values of the eyes.
bool UserIdentification::foundEyes(){
	return foundFace() && (!_rightEye.isEmpty() || !_leftEye.isEmpty());
}

// Input: Eye to check.
// Output: Return true if the eye was found.
//-Check if the object contain value of the eye.
bool UserIdentification::foundOneEye(bool eye_side) {
	return foundFace() && (eye_side ? _rightEye.isEmpty() && !_leftEye.isEmpty() : !_rightEye.isEmpty() && _leftEye.isEmpty());
}

UserIdentification pirior;
// Input: Pirior user identification values, eye to check.
// Output: Return true if user winked, false if didn't.
//-Checks if user winked based on the values of the current identification and the pirior one. 
bool UserIdentification::userWinked(bool eye_side) {
	bool result = ((foundFace() && foundOneEye(eye_side)) && (pirior.foundFace() && !pirior.foundOneEye(eye_side)));
	pirior = *this;
	return result;
}

// Input: Pirior user identification values.
// Output: Return true if user blinked, false if didn't.
//-Checks if user blinked based on the values of the current identification and the pirior one. 
bool UserIdentification::userBlinked(UserIdentification& pirior){
	//-If current frame had eyes, and the pirior didn't, return 'blink' approval.
	return ((foundFace() && foundEyes()) && (pirior.foundFace() && !pirior.foundEyes()));
}

// Output: Return the face center cordinations.
Point UserIdentification::getFaceCenter() {
	return Point(_face.x + (_face.width / 2), _face.y + (_face.height / 2));
}

// Input: ID of which eye center to return.
// Output: Center of the wished eye.
Point UserIdentification::getEyeCenter(EyeSide eye_side) {
	return eye_side ? getEyeCenter(_rightEye._eye_rect) : getEyeCenter(_leftEye._eye_rect);
}

// Input: Rect of the eye.
// Output: Center of the wished eye.
Point UserIdentification::getEyeCenter(Rect eye_rect) {
	return Point(_face.x + eye_rect.x + eye_rect.width / 2, _face.y + eye_rect.y + eye_rect.height / 2);
}

// Input: ID of which eye corner to return
// Output: Corner of the wished eye.
Point UserIdentification::getEyeCorner(EyeSide eye_side) {
	return eye_side ? Point(_face.x + _rightEye._eye_rect.x, _face.y + _rightEye._eye_rect.y + _rightEye._eye_rect.height / 2) : Point(_face.x + _leftEye._eye_rect.x + _leftEye._eye_rect.width, _face.y + _leftEye._eye_rect.y + _leftEye._eye_rect.height / 2);
}