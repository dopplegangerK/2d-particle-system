#include "Enemy.h"
#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

Rocket* Enemy::player = nullptr;
std::vector<std::shared_ptr<Enemy>> Enemy::all_enemies = std::vector<std::shared_ptr<Enemy>>();

int Enemy::height = 0;
int Enemy::width = 0;
SDL_Texture* Enemy::tex = nullptr;

Enemy::Enemy(int x, int y) : Particle(x, y), direction{ 0, 0 } {
	speed = rand() % (max_speed - min_speed) + min_speed;
	rect = new SDL_Rect;
	rect->w = width;
	rect->h = height;
}

Enemy::~Enemy() {
	delete rect;
}

void Enemy::step(double seconds) {
	Vector acceleration(Point{ x, y }, player->getLoc());
	acceleration = acceleration.scaleTo(speed);

	// check for nearby enemies (again, replace this with spatial data structure later)
	Vector repel_force;
	double distance_squared;
	for (std::shared_ptr<Enemy> e : Enemy::all_enemies) {
		if (e.get() == this)
			continue;

		distance_squared = distanceSquared(Point{ x, y }, Point{ e->x, e->y });
		if (distance_squared < (max_affecting_distance * max_affecting_distance)) {
			repel_force = repel_force + Vector(Point{ e->x, e->y }, Point{ x, y }).scaleTo(repel * repel / distance_squared);
			//repel_force = repel_force + Vector(Point{ e->x, e->y }, Point{ x, y }).scaleTo(repel * (1 - sqrt(distance_squared) / max_affecting_distance));
		}
	}
	if (repel_force.getLength() > 0) {
		repel_force.scaleBy(1 / (all_enemies.size() - 1));
		acceleration = acceleration + repel_force;

		if (repel_force.getLength() > speed * 2) {
			repel_force.scaleTo(speed * 2);
		}
	}

	Vector velocity = acceleration/*.scaleTo(speed)*/;
	x += velocity.getX();
	y += velocity.getY();
}

void Enemy::draw(SDL_Renderer* ren) {
	//filledCircleColor(ren, x, y, enemy_width/2, 0xff0000ff);
	rect->x = x - width / 2;
	rect->y = y - height / 2;
	SDL_RenderCopy(ren, tex, NULL, rect);
}

EnemySpawn::EnemySpawn(int screenWidth, int screenHeight) :
	RingParticleSource( screenWidth / 2, screenHeight / 2,
		(int)sqrt(distanceSquared(Point{ screenWidth/2, screenHeight/2 }, Point{ 0, 0 })) + 50,
		10, true, true) {}

void EnemySpawn::generate_new_particles(int num) {
	int n = particles.size();
	RingParticleSource::generate_new_particles(num);
	size_t x = n;
	std::list<std::shared_ptr<Enemy>>::reverse_iterator it = particles.rbegin();
	while (x < particles.size()) {
		Enemy::all_enemies.push_back(*it);
		it++;
		x++;
	}
}

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