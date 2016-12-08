#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Rocket.h"
#include "Explosion.h"
#include "Vector.h"
#include "Bullet.h"
#include <Box2D\Box2D.h>
#include <Particles.h>
#include <SDL.h>
#include <vector>

//type = 1
class Enemy: public PhysicsParticle {
protected:
	//keep the player's location so we know where to go
	static Rocket* player;
	//maximum distance two enemies can be from each other before they start to repel
	static constexpr double max_affecting_distance = 75;
	//coefficient for how much enemies repel each other
	static constexpr double repel = 600;

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

	Enemy(int x, int y, int w, int h, SDL_Texture* tex, int hp);
	virtual ~Enemy() {}

	virtual void draw(SDL_Renderer* ren);
	virtual void step(double seconds) = 0;
	virtual bool is_dead() const { return hp <= 0; }

	virtual void hit(int damage);
	virtual int pointValue() = 0;

	static std::shared_ptr<Enemy> createParticleAt(int x, int y);
	static void setPlayer(Rocket* r) { player = r; }
	static void setPhysicsWorld(b2World* w) { enemy_world = w; }
};

class GreenEnemy : public Enemy {
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../demo/sprites/ufoGreen.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	GreenEnemy(int x, int y);
	virtual ~GreenEnemy() {}

	virtual void step(double seconds);

	virtual int pointValue() { return 5; }
};

class RedEnemy : public Enemy {
private:
	static constexpr double max_shot_time = 0.75;
	const Point target;

	double shot_time = 0.5;
	std::shared_ptr<BulletSource<EnemyBullet>> gun;

	void shoot();
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../demo/sprites/ufoRed.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	RedEnemy(int x, int y);
	virtual ~RedEnemy() {}
	virtual bool is_dead() const;
	virtual void draw(SDL_Renderer* ren);

	virtual void step(double seconds);

	virtual int pointValue() { return 20; }
};

class EnemySpawn : public RingParticleSource<Enemy> {
private:
	const int time_to_spawn = 1;
	double time = 0;

	std::list<Explosion> explosions;

	virtual void generate_new_particles(int num);
public:
	EnemySpawn(int screenWidth, int screenHeight);
	~EnemySpawn() {}

	virtual void initialize_particles();
	virtual void step(double seconds);
	virtual void step_explosions(double seconds);
	virtual void draw_particles(SDL_Renderer* ren);
	virtual void clear();
};

#endif