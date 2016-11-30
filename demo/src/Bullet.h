#ifndef _BULLET_H_
#define _BULLET_H_

#include <Particles.h>
#include <iostream>

// type = 3
class Bullet : public PhysicsTrajectoryParticle {
private:
	static constexpr int SPEED = 100;
	bool was_hit;

	SDL_Rect* rect;

	static b2World* world;
	static b2Body* makeBulletBody(int x, int y);
	static b2Shape* makeBulletShape();
public:
	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/laserRed07.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	static void setPhysicsWorld(b2World* w);

	static std::shared_ptr<Bullet> createParticleAt(int x, int y);

	Bullet(int x, int y, double dir);
	virtual ~Bullet() { std::cout << "Killing bullet\n"; }
	virtual void draw(SDL_Renderer* ren);
	virtual bool is_dead() const;
	virtual void hit();
};

class BulletSource : public PointParticleSource<Bullet> {
public:
	BulletSource(int x, int y);
	virtual ~BulletSource() {}
	virtual void fire(double direction);
};

#endif