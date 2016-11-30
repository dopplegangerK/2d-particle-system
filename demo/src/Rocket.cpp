#include "Rocket.h"
#include "Particle.h"

#include <iostream>

int Rocket::height = 0;
int Rocket::width = 0;
SDL_Texture* Rocket::tex = nullptr;

b2World* Rocket::world = nullptr;

Rocket::Rocket() : Rocket(Point{ 0, 0 }, 0) {}

Rocket::Rocket(const Rocket& r) : loc{ r.loc.x, r.loc.y }, direction{ r.direction },
		rect{ new SDL_Rect }, gun(r.gun) {
	rect->w = width;
	rect->h = height;
}

Rocket::Rocket(Point loc, double dir) :
		loc{ loc.x, loc.y }, direction{ Vector::getDir(dir).scaleTo(speed) }, gun{ loc.x, loc.y } {
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
	bodyDef.position.Set((float32)loc.x/10, (float32)loc.y/10);
	bodyDef.type = b2_dynamicBody;
	body = world->CreateBody(&bodyDef);
	//shape (create a triangle)
	b2Vec2 vertices[3];
	vertices[0].Set(-height/20.0f, -width / 20.0f);
	vertices[1].Set(width/20.0f, 0.0f);
	vertices[2].Set(height/20.0f, -width/20.0f);
	b2PolygonShape* pShape = new b2PolygonShape();
	pShape->Set(vertices, 3);
	shape = pShape;
	//create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
	fixtureDef.restitution = 1;
	fixtureDef.userData = new PhysicsData{ 2, this };
	fixture = body->CreateFixture(&fixtureDef);
	//filter out collisions with bullets
	b2Filter filter;
	filter.categoryBits = 0x0002;
	filter.maskBits = ~0x0004;
	fixture->SetFilterData(filter);
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
	gun = r.gun;
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
	loc.x = (int)(body->GetWorldCenter().x * 10);
	loc.y = (int)(body->GetWorldCenter().y * 10);
	body->SetLinearVelocity(b2Vec2((float32)direction.getX(), (float32)direction.getY()));
	body->SetTransform(body->GetPosition(), (float32)direction.getAngle());

	// update bullets
	if (fire) {
		gun.moveTo(loc.x, loc.y);
		gun.fire(direction.getAngle());
		fire = false;
	}
	gun.step(seconds);

	//update fire
	double angle = direction.getAngle();
	Point a{ (int)(loc.x - cos(angle) * height / 2.0), (int)(loc.y - sin(angle) * width / 2.0) };
	Point b{ (int)(loc.x - cos(angle) * height / 2.0), (int)(loc.y + sin(angle) * width / 2.0) };
	fire_source.moveTo(a, b);
	fire_source.step(seconds);
}

void Rocket::shoot() { fire = true; }

Point Rocket::getLoc() const { return loc; }

double Rocket::getDir() const { return direction.getAngle(); }

void Rocket::draw(SDL_Renderer* ren) {
	fire_source.draw_particles(ren);
	gun.draw_particles(ren);

	rect->x = loc.x - width / 2;
	rect->y = loc.y - height / 2;
	double angle = toDegrees(direction.getAngle()) + 90;
	SDL_RenderCopyEx(ren, tex, NULL, rect, angle, NULL, SDL_FLIP_NONE);
}

void Rocket::setPhysicsWorld(b2World* w) { world = w; }
