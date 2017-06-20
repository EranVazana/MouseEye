#include "CV_OperatorOverloading.h"

//======================================================================

Point operator/(Point& p, int n) {
	return Point((int)(p.x / n), (int)(p.y / n));
}

Point& operator/=(Point& p, int n)
{
	return p = p / n;
}

Point operator/(Point& p, XY_Amount nM) {
	return Point((int)(p.x / nM.first), (int)(p.y / nM.second));
}

Point& operator/=(Point& p, XY_Amount nM) {
	return p = p / nM;
}

Point operator+(Point& p, int n) {
	return Point(p.x + n, p.y + n);
}

Point& operator+=(Point& p, int n) {
	return p = p + n;
}

Point operator+(Point& p, XY_Amount nM) {
	return Point(p.x + nM.first, p.y + nM.second);
}

Point& operator+=(Point& p, XY_Amount nM) {
	return p = p + nM;
}

Point operator+(Point& p, Point other){
	return Point(p.x + other.x, p.y + other.y);
}
Point& operator+=(Point& p, Point other) {
	return p = p + other;
}

Point operator+(Point& p, Size s) {
	return Point(p.x + s.width, p.y + s.height);
}

Point& operator+=(Point& p, Size s) {
	return p = p + s;
}

Point operator-(Point& p, int n) {
	return Point(p.x - n, p.y - n);
}

Point& operator-=(Point& p, int n) {
	return p = p - n;
}

Point operator-(Point& p, XY_Amount nM) {
	return Point(p.x - nM.first, p.y - nM.second);
}

Point& operator-=(Point& p, XY_Amount nM) {
	return p = p - nM;
}

Point operator-(Point& p, Point other) {
	return Point(p.x + other.x, p.y + other.y);
}

Point& operator-=(Point& p, Point other) {
	return p = p - other;
}

Point operator-(Point& p, Size s) {
	return Point(p.x - s.width, p.y - s.height);
}

Point& operator-=(Point& p, Size s) {
	return p = p - s;
}

bool operator==(Point& p, Point other) {
	return p.x == other.x && p.y == other.y;
}

ostream& operator<< (ostream& os, const Point& p) {
	os << "(" << p.x << "," << p.y << ")";
	return os;
}

//======================================================================

Size operator/(Size& s, int n) {
	return Size(s.width / n, s.height / n);
}

Size operator/=(Size& s, int n) {
	return s = s / n;
}

Size operator+(Size& s, int n) {
	return Size(s.width + n, s.height + n);
}

Size operator+=(Size& s, int n) {
	return s = s + n;
}

Size operator-(Size& s, int n) {
	return Size(s.width - n, s.height - n);
}

Size operator-=(Size& s, int n) {
	return s = s - n;
}

//======================================================================

ostream& operator<< (ostream& os, const Size& s) {
	os << "(" << s.width << "," << s.height << ")";
	return os;
}