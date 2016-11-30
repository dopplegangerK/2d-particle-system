#pragma once
#include <cstdint>

#define PI 3.141592653589793238462643383279502884L

struct Point {
	int x;
	int y;
};

class Vector {
private:
	double x;
	double y;
	double length;
public:
	Vector();
	Vector(double x, double y);
	Vector(const Point start, const Point end);
	int getX() const;
	int getY() const;
	double getLength() const;
	double getAngle() const;
	void reset(int newX, int newY);
	Vector getUnit() const;
	Vector getPerp() const;
	Vector getOpposite() const;
	Vector scaleBy(double scale);
	Vector scaleTo(double length);

	Vector operator+ (const Vector& other) const {
		return Vector(x + other.x, y + other.y);
	}

	static Vector getDir(double angle);

	void flip();
};

Point moveBy(const Point p, const Vector dir);

Point moveOrigin(const Point p, const Point newOrigin);

double toDegrees(double radians);

double distanceSquared(const Point a, const Point b);

double distance(const Point a, const Point b);

Point rotate(const Point p, double angle, const Point center = { 0, 0 });
