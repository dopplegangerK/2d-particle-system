#include <cmath>
#include <cstdlib>
#include "Vector.h"

namespace Particles {

	Vector::Vector() : x{ 0 }, y{ 0 }, length{ 0 } {}

	Vector::Vector(double xCoord, double yCoord) {
		x = xCoord;
		y = yCoord;
		length = sqrt(x * x + y * y);
	}

	Vector::Vector(const Point start, const Point end) {
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

	Point moveBy(const Point p, const Vector dir) {
		return Point{ p.x + dir.getX(), p.y + dir.getY() };
	}

	Point moveOrigin(const Point p, const Point newOrigin) {
		return{ p.x + newOrigin.x, p.y + newOrigin.y };
	}

	double toDegrees(double radians) { return radians * 180 / PI; }

	double distanceSquared(const Point a, const Point b) {
		float xDiff = a.x - b.x;
		float yDiff = a.y - b.y;
		return xDiff * xDiff + yDiff * yDiff;
	}

	double distance(const Point a, const Point b) { return sqrt(distanceSquared(a, b)); }

	Point rotate(const Point p, double angle, const Point center) {
		Vector v(center, p);
		//get the coordinates of p relative to center
		Point rotated{ p.x - center.x, p.y - center.y };
		//rotate
		angle = v.getAngle() + angle;
		double dist = distance(rotated, { 0, 0 });
		rotated.x = (float)(cos(angle) * dist);
		rotated.y = (float)(sin(angle) * dist);
		//put back in world coordinates
		rotated.x += center.x;
		rotated.y += center.y;
		return rotated;
	}

	Point getPointOnRing(Point center, float radius) {
		double angle = 2 * PI * ((rand() % 200) / 200.0);
		return Point{ (float)(center.x + cos(angle) * radius), (float)(center.y + sin(angle) * radius) };
	}
}
