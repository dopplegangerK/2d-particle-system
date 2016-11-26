#include "Rocket.h"

#include <iostream>

int Rocket::height = 0;
int Rocket::width = 0;
SDL_Texture* Rocket::tex = nullptr;

Rocket::Rocket() : loc{ 0, 0 }, direction{}, rect{ nullptr } {}

Rocket::Rocket(const Rocket& r) : loc{ r.loc.x, r.loc.y }, direction{ r.direction }, rect{ new SDL_Rect } {}

Rocket::Rocket(Point loc, double dir) :
		loc{ loc.x, loc.y }, direction{ Vector::getDir(dir).scaleTo(speed) } {
	rect = new SDL_Rect;
	rect->w = width;
	rect->h = height;
}

Rocket::~Rocket() {
	delete rect;
}

Rocket& Rocket::operator= (const Rocket& r) {
	loc = r.loc;
	direction = r.direction;
	if (rect == nullptr && r.rect != nullptr) {
		rect = new SDL_Rect;
		rect->w = width;
		rect->h = height;
	}
	return *this;
}

void Rocket::setDir(double dir) {
	direction = Vector::getDir(dir).scaleTo(speed);
}

void Rocket::step(double seconds) {
	loc = moveBy(loc, direction);
	//std::cout << "location: " << loc.x << ", " << loc.y << "\n";
}

Point Rocket::getLoc() const { return loc; }

double Rocket::getDir() const { return direction.getAngle(); }

void Rocket::draw(SDL_Renderer* ren) {
	rect->x = loc.x - width / 2;
	rect->y = loc.y - height / 2;
	double angle = toDegrees(direction.getAngle()) + 90;
	SDL_RenderCopyEx(ren, tex, NULL, rect, angle, NULL, SDL_FLIP_NONE);
}