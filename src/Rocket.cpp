#include "Rocket.h"

#include <iostream>

Rocket::Rocket(Point loc, double dir) :
	loc{ loc.x, loc.y }, direction{ Vector::getDir(dir).scaleTo(speed) } {}

void Rocket::setDir(double dir) {
	direction = Vector::getDir(dir).scaleTo(speed);
}

void Rocket::step(double seconds) {
	loc = moveBy(loc, direction);
	//std::cout << "location: " << loc.x << ", " << loc.y << "\n";
}

Point Rocket::getLoc() const { return loc; }

double Rocket::getDir() const { return direction.getAngle(); }