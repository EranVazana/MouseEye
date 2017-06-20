#pragma once
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

typedef pair<int, int> XY_Amount;

//-Class used for oveloading OpenCV objects for convinient.
class CV_OperatorOverloading {
public:
	//======================================================================

	friend Point operator/(Point&, int);
	friend Point& operator/=(Point&, int);
	friend Point operator/(Point&, XY_Amount);
	friend Point& operator/=(Point&, XY_Amount);

	friend Point operator+(Point&, int);
	friend Point& operator+=(Point&, int);
	friend Point operator+(Point&, XY_Amount);
	friend Point& operator+=(Point&, XY_Amount);
	friend Point operator+(Point&, Point);
	friend Point& operator+=(Point&, Point);
	friend Point operator+(Point&, Size);
	friend Point& operator+=(Point&, Size);

	friend Point operator-(Point&, int);
	friend Point& operator-=(Point&, int);
	friend Point operator-(Point&, XY_Amount);
	friend Point& operator-=(Point&, XY_Amount);
	friend Point operator-(Point&, Point);
	friend Point& operator-=(Point&, Point);
	friend Point operator-(Point&, Size);
	friend Point& operator-=(Point&, Size);

	friend bool operator==(Point&, Point);

	friend ostream& operator<< (ostream&, const Point&);

	//======================================================================

	friend Size operator/(Size&, int);
	friend Size operator/=(Size&, int);

	friend Size operator+(Size&, int);
	friend Size operator+=(Size&, int);

	friend Size operator-(Size&, int);
	friend Size operator-=(Size&, int);

	friend ostream& operator<< (ostream&, const Size&);
};