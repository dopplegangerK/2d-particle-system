#include "Rocket.h"
#include "Particle.h"

int Rocket::height = 0;
int Rocket::width = 0;
SDL_Texture* Rocket::tex = nullptr;

b2World* Rocket::world = nullptr;

Rocket::Rocket() : Rocket(Point{ 0, 0 }, 0) {}
Mix_Chunk* Rocket::damage_sound = nullptr;

#define corner2 { -height / 2.0f, -width * 0.2f }
#define corner1 { -height / 2.0f, width * 0.2f }

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
	//destroy everything first
	if (body != nullptr) {
		world->DestroyBody(body);
		delete shape;
	}

	//body
	b2BodyDef bodyDef;
	bodyDef.position.Set((float32)loc.x/10, (float32)loc.y/10);
	bodyDef.type = b2_dynamicBody;
	body = world->CreateBody(&bodyDef);

	//shape (create a triangle)
	b2Vec2 vertices[3];
	vertices[0].Set(-height/20.0f, width / 20.0f * 0.8f);
	vertices[1].Set(0.0f, -width / 20.0f * 0.8f);
	vertices[2].Set(height/20.0f, width/20.0f * 0.8f);
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

	//Set our collision category
	b2Filter filter;
	filter.categoryBits = collision_category;
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
	explosion = r.explosion;
	dead = r.dead;
	life = r.life;
	fire = r.fire;
	my_hit_time = r.my_hit_time;
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
	if (dead) {
		explosion->step(seconds);
		return;
	}

	if (my_hit_time >= 0) {
		my_hit_time += seconds;
		if (my_hit_time >= hit_blink_time)
			my_hit_time = -1;
	}

	if (body == nullptr)
		return;

	loc.x = body->GetWorldCenter().x * 10;
	loc.y = body->GetWorldCenter().y * 10;
	if(body->GetContactList() == nullptr) {
		body->SetTransform(body->GetPosition(), (float32)(direction.getAngle() + PI/2));
		body->SetLinearVelocity(b2Vec2((float32)direction.getX(), (float32)direction.getY()));
	} else {
		direction = Vector::getDir(body->GetAngle() - PI/2).scaleTo(speed);
	}

	// update bullets
	if (fire) {
		gun.moveTo(loc.x, loc.y);
		gun.fire(direction.getAngle());
		fire = false;
	}
	gun.step(seconds);

	//update fire
	double angle = direction.getAngle();
	Point b = moveOrigin(rotate(corner1, angle), loc);
	Point a = moveOrigin(rotate(corner2, angle), loc);
	fire_source.moveTo(a, b);
	fire_source.step(seconds);
}

void Rocket::shoot() { fire = true; }

void Rocket::hit() {
	if (!canHit())
		return;

	my_hit_time = 0;
	life--;
	if (life == 0)
		explode();
	else
		Mix_PlayChannel(-1, damage_sound, 3);
}

void Rocket::kill() {
    if(!canHit())
        return;
    life = 0;
    explode();
}

bool Rocket::canHit() {
	return my_hit_time < 0;
}

void Rocket::explode() {
	if (!dead) {
		explosion = std::make_shared<Explosion>(loc.x, loc.y);
		Explosion::playSound();
		dead = true;
	}
}

int Rocket::getLife() { return life; }

Point Rocket::getLoc() const { return loc; }

double Rocket::getDir() const { return direction.getAngle(); }

void Rocket::draw(SDL_Renderer* ren) {
	if (dead) {
		explosion->drawParticles(ren);
		return;
	}

	gun.drawParticles(ren);

	if (my_hit_time < 0 || (int)(my_hit_time*8) % 2 == 0) {
		fire_source.drawParticles(ren);

		rect->x = (int)(loc.x - width / 2);
		rect->y = (int)(loc.y - height / 2);
		double angle = toDegrees(direction.getAngle()) + 90;
		SDL_RenderCopyEx(ren, tex, NULL, rect, angle, NULL, SDL_FLIP_NONE);
	}
}

void Rocket::setPhysicsWorld(b2World* w) { world = w; }
