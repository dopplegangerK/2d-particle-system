#include "Enemy.h"
#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

Rocket* Enemy::player = nullptr;
std::vector<std::shared_ptr<Enemy>> Enemy::all_enemies = std::vector<std::shared_ptr<Enemy>>();

Enemy::Enemy(int x, int y) : Particle(x, y), direction{ 0, 0 } {
	speed = rand() % (max_speed - min_speed) + min_speed;
}

void Enemy::step(double seconds) {
	Vector acceleration(Point{ x, y }, player->getLoc());
	acceleration = acceleration.scaleTo(speed);

	// check for nearby enemies (again, replace this with spatial data structure later)
	Vector repel_force;
	double distance_squared;
	for (std::shared_ptr<Enemy> e : Enemy::all_enemies) {
		distance_squared = distanceSquared(Point{ x, y }, Point{ e->x, e->y });
		if (distance_squared < (max_affecting_distance * max_affecting_distance)) {
			repel_force = Vector(Point{ e->x, e->y }, Point{ x, y });
			repel_force = repel_force.scaleTo(repel/* / repel_force.getLength()*/);
			acceleration = acceleration + repel_force;
		}
	}

	Vector velocity = acceleration/*.scaleTo(speed)*/;
	x += velocity.getX();
	y += velocity.getY();

	std::cout << "New enemy location: " << x << ", " << y << "\n";
}

void Enemy::draw(SDL_Renderer* ren) {
	filledCircleColor(ren, x, y, 15, 0xff0000ff);
}

EnemySpawn::EnemySpawn(int screenWidth, int screenHeight) :
	RingParticleSource( screenWidth / 2, screenHeight / 2,
		(int)sqrt(distanceSquared(Point{ screenWidth/2, screenHeight/2 }, Point{ 0, 0 })) + 50,
		10, true, true) {}

void EnemySpawn::initialize_particles() {
	generate_new_particles(1);
}

void EnemySpawn::step(double seconds) {
	//spawn new enemy if we've passed the given timestep
	time += seconds;
	if (time >= time_to_spawn) {
		time = 0;
		generate_new_particles(1);
	}

	//step all enemies
	std::list<std::shared_ptr<Enemy>>::iterator it = particles.begin();
	while (it != particles.end()) {
		std::shared_ptr<Enemy> enemy = *it;
		enemy->step(seconds);
		if (enemy->is_dead()) {
			particles.erase(it);
		}
		else {
			it++;
		}
	}
}