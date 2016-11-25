#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Rocket.h"
#include "Particle.h"
#include "RingParticleSource.h"
#include "Vector.h"
#include <vector>
#include <SDL.h>

class Enemy: public Particle {
private:
	// Replace this with some sort of spacial data structure later (quadtree?)
	static std::vector<std::shared_ptr<Enemy>> all_enemies;
	//keep the player's location so we know where to go
	static Rocket* player;
	//maximum distance two enemies can be from each other before they start to repel
	static constexpr double max_affecting_distance = 100.0;
	//coefficient for how much enemies repel each other
	static constexpr double repel = 100;

	static constexpr int max_speed = 5;
	static constexpr int min_speed = 3;

	Vector direction;
	int speed;
	int hp = 3;
public:
	friend class EnemySpawn;

	Enemy(int x, int y);
	virtual ~Enemy() {}

	virtual void draw(SDL_Renderer* ren);
	virtual void step(double seconds);
	virtual bool is_dead() const { return hp == 0; }

	static std::shared_ptr<Enemy> createParticleAt(int x, int y) { return std::make_shared<Enemy>(x, y); }
	static void setPlayer(Rocket* r) { player = r; }
};

class EnemySpawn : public RingParticleSource<Enemy> {
private:
	const int time_to_spawn = 1;
	double time = 0;

	virtual void generate_new_particles(int num);
public:
	EnemySpawn(int screenWidth, int screenHeight);
	~EnemySpawn() {}

	virtual void initialize_particles();
	virtual void step(double seconds);
};

#endif