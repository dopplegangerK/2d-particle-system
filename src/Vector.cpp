#include <cmath>
#include "Vector.h"

Vector::Vector() : x{ 0 }, y{ 0 }, length{ 0 } {}

Vector::Vector(double xCoord, double yCoord) {
	x = xCoord;
	y = yCoord;
	length = sqrt(x * x + y * y);
}

Vector::Vector(Point start, Point end) {
	x = (double)(end.x - start.x);
	y = (double)(end.y - start.y);
	length = sqrt(x * x + y * y);
}

int Vector::getX() const { return (int)x; }

int Vector::getY() const { return (int)y; }

double Vector::getLength() const { return length; }

double Vector::getAngle() const { return atan2(y, x); }

void Vector::reset(int newX, int newY) {
	x = newX;
	y = newY;
	length = sqrt(x * x + y * y);
}

Vector Vector::getUnit() const {
	return Vector{ x / length, y / length };
}

Vector Vector::getPerp() const {
	return Vector{ 0 - y, x };
}

Vector Vector::getOpposite() const {
	return Vector{ 0 - x, 0 - y };
}

Vector Vector::scaleBy(double scale) {
	return Vector{ x * scale, y * scale };
}

Vector Vector::scaleTo(double newLength) {
	return Vector{ (x * newLength) / length, (y * newLength) / length };
}

Vector Vector::getDir(double angle) {
	return Vector{ cos(angle), sin(angle) };
}

void Vector::flip() {
	x = 0 - x;
	y = 0 - y;
}

Point moveBy(Point p, Vector dir) {
	return Point{ p.x + dir.getX(), p.y + dir.getY() };
}

double toDegrees(double radians) { return radians * 180 / PI; }

double distanceSquared(Point a, Point b) {
	int xDiff = a.x - b.x;
	int yDiff = a.y - b.y;
	return xDiff * xDiff + yDiff * yDiff;
}