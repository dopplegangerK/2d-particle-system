#include "Bullet.h"

#include "ParticleSource.h"
#include <memory>

/**********
 * Bullet *
 **********/

b2World* Bullet::world = nullptr;

Bullet::Bullet(int x, int y, double dir, int w, int h, SDL_Texture* tex) :
	PhysicsTrajectoryParticle(x, y, dir, SPEED, world, makeBulletBody(x, y, dir), makeBulletShape(w, h), 3, 0, 0, 1, true),
	was_hit(false), myTex(tex) {
	rect.w = w;
	rect.h = h;
}

b2Body* Bullet::makeBulletBody(int x, int y, double dir) {
	b2BodyDef bodyDef;
	bodyDef.position.Set((float32)x/10, (float32)y/10);
	bodyDef.type = b2_kinematicBody;
	bodyDef.bullet = true;
	bodyDef.angle = (float32)dir;
	return world->CreateBody(&bodyDef);
}

b2Shape* Bullet::makeBulletShape(int w, int h) {
	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox((float32)h/20, (float32)w/20);
	return shape;
}

void Bullet::draw(SDL_Renderer* ren) {
	rect.x = (int)x - rect.w / 2;
	rect.y = (int)y - rect.h / 2;
	SDL_RenderCopyEx(ren, myTex, NULL, &rect, toDegrees(angle) + 90, NULL, SDL_FLIP_NONE);

	//draw corners of polygon for debugging purposes
	/*
	b2Transform t = body->GetTransform();
	for (int k = 0; k < 4; k++) {
		b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(k);
		Point p = rotate({ (int)(vertex.x * 10), (int)(vertex.y * 10) }, t.q.GetAngle());
		filledCircleColor(ren, p.x + x, p.y + y, 3, 0xff00ffff);
	}
	*/
}

bool Bullet::is_dead() const {
	return was_hit || (x < 0 || x >= 1024) || (y < 0 || y >= 640);
}

void Bullet::hit() {
	was_hit = true;
}

void Bullet::setPhysicsWorld(b2World* w) { world = w; }

/*****************
 * Player Bullet *
 *****************/

int PlayerBullet::width = 0;
int PlayerBullet::height = 0;
SDL_Texture* PlayerBullet::tex = nullptr;

PlayerBullet::PlayerBullet(int x, int y, double dir) : Bullet(x, y, dir, width, height, tex) {
	//filter out collisions with ship
	b2Filter filter;
	filter.categoryBits = 0x0004;
	filter.maskBits = ~0x0002;
	fixture->SetFilterData(filter);
}

/****************
 * Enemy Bullet *
 ****************/

int EnemyBullet::width = 0;
int EnemyBullet::height = 0;
SDL_Texture* EnemyBullet::tex = nullptr;

EnemyBullet::EnemyBullet(int x, int y, double dir) : Bullet(x, y, dir, width, height, tex) {
	//filter out collisions with enemies
	b2Filter filter;
	filter.categoryBits = 0x0010;
	filter.maskBits = ~0x0008;
	fixture->SetFilterData(filter);
}