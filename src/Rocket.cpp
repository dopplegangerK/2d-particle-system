#include "Rocket.h"

#include <iostream>

int Rocket::height = 0;
int Rocket::width = 0;
SDL_Texture* Rocket::tex = nullptr;

b2World* Rocket::world = nullptr;

Rocket::Rocket() : Rocket(Point{ 0, 0 }, 0) {}

Rocket::Rocket(const Rocket& r) : loc{ r.loc.x, r.loc.y }, direction{ r.direction }, rect{ new SDL_Rect } {
	rect->w = width;
	rect->h = height;
}

Rocket::Rocket(Point loc, double dir) :
		loc{ loc.x, loc.y }, direction{ Vector::getDir(dir).scaleTo(speed) } {
	rect = new SDL_Rect;
	rect->w = width;
	rect->h = height;

	//set up physics stuff
	if (world == nullptr) {
		body = nullptr;
		shape = nullptr;
	} else {
		makePhysicsAttributes();
	}
}

void Rocket::makePhysicsAttributes() {
	//body
	b2BodyDef bodyDef;
	bodyDef.position.Set((float32)loc.x, (float32)loc.y);
	bodyDef.type = b2_kinematicBody;
	body = world->CreateBody(&bodyDef);
	//shape (create a triangle)
	b2Vec2 vertices[3];
	vertices[0].Set((float32)height, (float32)width / 2);
	vertices[1].Set(0, (float32)width);
	vertices[2].Set(0, 0);
	b2PolygonShape* pShape = new b2PolygonShape();
	pShape->Set(vertices, 3);
	shape = pShape;
	//create fixture
	body->CreateFixture(shape, 0);
}

Rocket::~Rocket() {
	delete rect;
	if (world != nullptr && body != nullptr) {
		world->DestroyBody(body);
		if (shape != nullptr)
			delete shape;
	}
}

Rocket& Rocket::operator= (const Rocket& r) {
	loc = r.loc;
	direction = r.direction;
	if (rect == nullptr && r.rect != nullptr) {
		rect = new SDL_Rect;
	}
	rect->w = width;
	rect->h = height;

	makePhysicsAttributes();
	return *this;
}

void Rocket::setDir(double dir) {
	direction = Vector::getDir(dir).scaleTo(speed);
}

void Rocket::step(double seconds) {
	if (body == nullptr)
		return;
	loc.x = (int)body->GetWorldCenter().x;
	loc.y = (int)body->GetWorldCenter().y;
	body->SetLinearVelocity(b2Vec2((float32)direction.getX(), (float32)direction.getY()));
	//loc = moveBy(loc, direction);
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

void Rocket::setPhysicsWorld(b2World* w) { world = w; }