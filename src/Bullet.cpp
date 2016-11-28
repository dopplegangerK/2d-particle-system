#include "Bullet.h"

#include "ParticleSource.h"
#include <memory>

b2World* Bullet::world = nullptr;
int Bullet::width = 0;
int Bullet::height = 0;
SDL_Texture* Bullet::tex = nullptr;


Bullet::Bullet(int x, int y, double dir) :
	PhysicsTrajectoryParticle(x, y, dir, SPEED, world, makeBulletBody(x, y), makeBulletShape(), 3, 0, 0, 1, false),
	rect(new SDL_Rect()), was_hit(false) {
	rect->w = width;
	rect->h = height;

	//filter out collisions with ship
	b2Filter filter;
	filter.categoryBits = 0x0004;
	filter.maskBits = 0x0001;
	fixture->SetFilterData(filter);
}

b2Body* Bullet::makeBulletBody(int x, int y) {
	b2BodyDef bodyDef;
	bodyDef.position.Set((float32)x/10, (float32)y/10);
	bodyDef.type = b2_kinematicBody;
	bodyDef.bullet = true;
	return world->CreateBody(&bodyDef);
}

b2Shape* Bullet::makeBulletShape() {
	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox((float32)width/10, (float32)height/10);
	return shape;
}

void Bullet::draw(SDL_Renderer* ren) {
	rect->x = x - width / 2;
	rect->y = y - height / 2;
	SDL_RenderCopyEx(ren, tex, NULL, rect, toDegrees(angle) + 90, NULL, SDL_FLIP_NONE);
}

bool Bullet::is_dead() const {
	return was_hit || (x < 0 || x >= 1024) || (y < 0 || y >= 640);
}

void Bullet::hit() {
	was_hit = true;
}

std::shared_ptr<Bullet> Bullet::createParticleAt(int x, int y) {
	return std::make_shared<Bullet>(x, y, 0);
}

void Bullet::setPhysicsWorld(b2World* w) { world = w; }

BulletSource::BulletSource(int x, int y) : PointParticleSource(x, y, 0, true, false) {}

void BulletSource::fire(double direction) {
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(x, y, direction);
	particles.push_back(bullet);
}