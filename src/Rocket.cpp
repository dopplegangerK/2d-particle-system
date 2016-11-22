#include "Rocket.h"

Rocket::Rocket(Point loc, double dir) :
	loc{ loc.x, loc.y }, direction{ dir } {}

void Rocket::setDir(double dir) { direction = dir; }

void Rocket::step(double seconds) {
	Vector moveVector = Vector::getDir(direction).scaleTo(speed * seconds);
	loc = moveBy(loc, moveVector);
}

Point Rocket::getLoc() const { return loc; }

double Rocket::getDir() const { return direction; }