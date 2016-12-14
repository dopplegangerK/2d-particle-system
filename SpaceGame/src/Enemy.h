#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Rocket.h"
#include "Explosion.h"
#include "Vector.h"
#include "Bullet.h"
#include <Box2D/Box2D.h>
#include <Particles.h>
#include <SDL.h>
#include <vector>

using namespace Particles;

//type = 1
class Enemy: public Particles::PhysicsParticle {
protected:
	//keep the player's location so we know where to go
	static Rocket* player;

	static constexpr int max_speed = 20;
	static constexpr int min_speed = 10;

	Vector direction;
	int speed;
	int hp;

	SDL_Rect rect;
	SDL_Texture* myTex;

	static b2World* enemy_world;

	static b2Body* makeEnemyBody(int x, int y);
	static b2Shape* makeEnemyShape(int r);
public:
	friend class EnemySpawn;

	static constexpr uint16_t collision_category = 0x0008;

	Enemy(float x, float y, int w, int h, SDL_Texture* tex, int hp);
	virtual ~Enemy() {}

	virtual void draw(SDL_Renderer* ren);
	virtual void step(double seconds) = 0;
	virtual bool isDead() const { return hp <= 0; }

	virtual void hit(int damage);
	virtual int pointValue() = 0;

	static std::shared_ptr<Enemy> createParticleAt(float x, float y);
	static void setPlayer(Rocket* r) { player = r; }
	static void setPhysicsWorld(b2World* w) { enemy_world = w; }
};

class GreenEnemy : public Enemy {
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../SpaceGame/sprites/ufoGreen.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	GreenEnemy(float x, float y);
	virtual ~GreenEnemy() {}

	virtual void step(double seconds);

	virtual int pointValue() { return 5; }
};

class RedEnemy : public Enemy {
private:
	static constexpr double max_shot_time = 1.0;
	const Point target;

	double shot_time = 0.2;
	std::shared_ptr<BulletSource<EnemyBullet>> gun;

	void shoot();
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../SpaceGame/sprites/ufoRed.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	RedEnemy(float x, float y);
	virtual ~RedEnemy() {}
	virtual bool isDead() const;
	virtual void draw(SDL_Renderer* ren);
	virtual void step(double seconds);

	virtual int pointValue() { return 20; }
};

class EnemySpawn : public Particles::RingParticleSource<Enemy> {
private:
    static constexpr double difficulty_coeff = 0.85;
    static constexpr int level_length = 12;

	int level = 0;

	double time_to_spawn = 1.5;
	double time = 0;
	double level_time = 0;

	std::list<Explosion> explosions;

	virtual void generateNewParticles(int num);
public:
	EnemySpawn(int screenWidth, int screenHeight);
	~EnemySpawn() {}

	virtual void initializeParticles();
	virtual void step(double seconds);
	virtual void stepExplosions(double seconds);
	virtual void drawParticles(SDL_Renderer* ren);
	virtual void clear();
};

#endif
