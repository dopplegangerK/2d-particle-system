#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Vector.h"

class Rocket {
private:
	const double speed = 5;	//pixels per millisec
	Point loc;
	Vector direction;
public:
	Rocket(Point loc, double direction = 0);
	void step(double seconds);
	void setDir(double dir);
	Point getLoc() const;
	double getDir() const;
};

#endif