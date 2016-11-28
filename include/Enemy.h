#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Rocket.h"
#include "Explosion.h"
#include "Particle.h"
#include "RingParticleSource.h"
#include "Vector.h"
#include "Box2D\Box2D.h"
#include <vector>
#include <SDL.h>

//type = 1
class Enemy: public PhysicsParticle {
private:
	// Replace this with some sort of spacial data structure later (quadtree?)
	//static std::vector<std::shared_ptr<Enemy>> all_enemies;
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
	int hp = 3;

	SDL_Rect* rect;

	static b2World* enemy_world;
	static b2Body* makeEnemyBody(int x, int y);
	static b2Shape* makeEnemyShape();
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../sprites/PNG/ufoGreen.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	friend class EnemySpawn;

	Enemy(int x, int y);
	virtual ~Enemy();

	virtual void draw(SDL_Renderer* ren);
	virtual void step(double seconds);
	virtual bool is_dead() const { return hp <= 0; }

	static std::shared_ptr<Enemy> createParticleAt(int x, int y) { return std::make_shared<Enemy>(x, y); }
	static void setPlayer(Rocket* r) { player = r; }
	static void setPhysicsWorld(b2World* w) { enemy_world = w; }

	void hit(int damage);
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
	virtual void draw_particles(SDL_Renderer* ren);
};

#endif