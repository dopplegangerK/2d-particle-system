#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Vector.h"
#include "Bullet.h"
#include "Fire.h"
#include "Explosion.h"
#include <SDL.h>
#include <Box2D\Box2D.h>

// type = 2
class Rocket {
private:
	static constexpr double speed = 30;

	Point loc;
	Vector direction;

	bool fire = false;

	RocketFireSource fire_source;

	SDL_Rect* rect;

	static b2World* world;
	b2Body* body = nullptr;
	b2Shape* shape = nullptr;
	b2Fixture* fixture = nullptr;

	BulletSource<PlayerBullet> gun;

	static constexpr double hit_blink_time = 1;
	double my_hit_time = -1;

	bool dead = false;
	std::shared_ptr<Explosion> explosion = nullptr;

	void makePhysicsAttributes();
public:
	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/playerShip3_blue.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	Rocket();
	Rocket(const Rocket& r);
	Rocket(Point loc, double direction = 0);
	~Rocket();
	Rocket& operator= (const Rocket& r);
	void step(double seconds);
	void shoot();
	void setDir(double dir);
	Point getLoc() const;
	double getDir() const;
	void draw(SDL_Renderer* ren);
	void hit();
	bool canHit();
	void explode();

	static void setPhysicsWorld(b2World* world);
};

#endif