#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Vector.h"

class Rocket {
private:
	Point loc;
	double direction;
	int speed;	//pixels per sec
public:
	Rocket(Point loc, double direction = 0);
	void step(double seconds);
	void setDir(double dir);
	Point getLoc() const;
	double getDir() const;
};

#endif