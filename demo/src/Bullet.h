#ifndef _BULLET_H_
#define _BULLET_H_

#include <Particles.h>
#include <iostream>
#include <SDL_Mixer.h>

// type = 3
class Bullet : public PhysicsTrajectoryParticle {
private:
	static constexpr int SPEED = 100;
	bool was_hit;

	SDL_Rect rect;
	SDL_Texture* myTex;

	static b2World* world;
	static b2Body* makeBulletBody(int x, int y, double dir);
	static b2Shape* makeBulletShape(int w, int h);
public:
	static void setPhysicsWorld(b2World* w);

	Bullet(int x, int y, double dir, int width, int height, SDL_Texture* tex);
	virtual ~Bullet() {}
	virtual void draw(SDL_Renderer* ren);
	virtual bool is_dead() const;
	virtual void hit();
};

class PlayerBullet : public Bullet {
public:
	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/laserRed07.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	PlayerBullet(int x, int y, double dir);
	virtual ~PlayerBullet() {}
	static std::shared_ptr<PlayerBullet> createParticleAt(int x, int y) { return std::make_shared<PlayerBullet>(x, y, 0); }
};

class EnemyBullet : public Bullet {
public:
	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/laserGreen13.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	EnemyBullet(int x, int y, double dir);
	virtual ~EnemyBullet() {}
	static std::shared_ptr<EnemyBullet> createParticleAt(int x, int y) { return std::make_shared<EnemyBullet>(x, y, 0); }
};

template <class B>
class BulletSource : public PointParticleSource<B> {
public:
	static Mix_Chunk* sound;
	static constexpr char* sound_path = "../../demo/sounds/laser.wav";

	BulletSource(int x, int y);
	virtual ~BulletSource() {}
	virtual void fire(double direction);
};

template<class B>
Mix_Chunk* BulletSource<B>::sound = nullptr;

template<class B>
BulletSource<B>::BulletSource(int x, int y) : PointParticleSource(x, y, 0, true, false) {}

template<class B>
void BulletSource<B>::fire(double direction) {
	if (x < 0 || x >= 1024 || y < 0 || y >= 640)
		return;

	Mix_PlayChannel(-1, sound, 0);
	std::shared_ptr<B> bullet = std::make_shared<B>(x, y, direction);
	particles.push_back(bullet);
}

#endif