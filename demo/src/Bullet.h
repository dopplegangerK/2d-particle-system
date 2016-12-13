#ifndef _BULLET_H_
#define _BULLET_H_

#include <Particles.h>
#include <SDL_mixer.h>

using namespace Particles;

// type = 3
class Bullet : public PhysicsTrajectoryParticle {
private:
	static constexpr int speed = 100;
	bool was_hit;

	SDL_Rect rect;
	SDL_Texture* my_tex;

	static b2World* world;
	static b2Body* makeBulletBody(int x, int y, double dir);
	static b2Shape* makeBulletShape(int w, int h);
public:
	static void setPhysicsWorld(b2World* w);

	Bullet(float x, float y, double dir, int width, int height, SDL_Texture* tex);
	virtual ~Bullet() {}
	virtual void draw(SDL_Renderer* ren);
	virtual bool isDead() const;
	virtual void hit();
};

class PlayerBullet : public Bullet {
public:
	static constexpr uint16_t collision_category = 0x0004;

	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/laserRed07.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	PlayerBullet(float x, float y, double dir);
	virtual ~PlayerBullet() {}
	static std::shared_ptr<PlayerBullet> createParticleAt(float x, float y) { return std::make_shared<PlayerBullet>(x, y, 0); }
};

class EnemyBullet : public Bullet {
public:
	static constexpr uint16_t collision_category = 0x0010;

	//drawing stuff
	static constexpr char* sprite = "../../demo/sprites/laserGreen13.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	EnemyBullet(float x, float y, double dir);
	virtual ~EnemyBullet() {}
	static std::shared_ptr<EnemyBullet> createParticleAt(float x, float y) { return std::make_shared<EnemyBullet>(x, y, 0); }
};

template <class B>
class BulletSource : public PointParticleSource<B> {
public:
	static Mix_Chunk* sound;
	static constexpr char* sound_path = "../../demo/sounds/laser.wav";

	BulletSource(float x, float y);
	BulletSource(Point p);
	virtual ~BulletSource() {}
	virtual void fire(double direction);
};

template<class B>
Mix_Chunk* BulletSource<B>::sound = nullptr;

template<class B>
BulletSource<B>::BulletSource(float x, float y) : PointParticleSource<B>(x, y, 0, true, false) {}

template<class B>
BulletSource<B>::BulletSource(Point p) : BulletSource(p.x, p.y) {}

template<class B>
void BulletSource<B>::fire(double direction) {
    float x = PointParticleSource<B>::p.x;
    float y = PointParticleSource<B>::p.y;
	if (x < 0 || x >= 1024 || y < 0 || y >= 640)
		return;

	Mix_PlayChannel(-1, sound, 0);
	std::shared_ptr<B> bullet = std::make_shared<B>(x, y, direction);
	ParticleSource<B>::particles.push_back(bullet);
}

#endif
