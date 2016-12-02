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
	const double speed = 30;
	Point loc;
	Vector direction;

	bool fire = false;

	RocketFireSource fire_source;

	SDL_Rect* rect;

	static b2World* world;
	b2Body* body;
	b2Shape* shape;
	b2Fixture* fixture;

	BulletSource gun;

	bool dead = false;
	Explosion* explosion = nullptr;

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
	void explode();

	static void setPhysicsWorld(b2World* world);
};

#endif