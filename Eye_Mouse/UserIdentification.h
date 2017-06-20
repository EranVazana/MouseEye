#pragma once
#include "Eye.h"

//-Class used to represent detection values of the user.
class UserIdentification {
public:
	#define FACE_RECT_COLOR CV_PINK

	UserIdentification(Rect);
	UserIdentification(Rect, Eye, Eye);
	UserIdentification();
	UserIdentification(const UserIdentification&);
	~UserIdentification();

	bool foundFace();
	bool foundEyes();
	bool foundOneEye(bool);

	bool userBlinked(UserIdentification&);
	bool userWinked (bool);

	Point getFaceCenter();

	Rect _face;
	Eye _rightEye, _leftEye;

private:
};