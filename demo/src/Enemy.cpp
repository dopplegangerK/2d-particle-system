#include "Enemy.h"
#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

Rocket* Enemy::player = nullptr;
//std::vector<std::shared_ptr<Enemy>> Enemy::all_enemies = std::vector<std::shared_ptr<Enemy>>();
b2World* Enemy::enemy_world = nullptr;

int Enemy::height = 0;
int Enemy::width = 0;
SDL_Texture* Enemy::tex = nullptr;

Enemy::Enemy(int x, int y) :
	PhysicsParticle(x, y, enemy_world, makeEnemyBody(x, y), makeEnemyShape(), 1, 0, 0, 1, true),
	direction{ 0, 0 } {
	speed = rand() % (max_speed - min_speed) + min_speed;
	rect = new SDL_Rect;
	rect->w = width;
	rect->h = height;
}

b2Body* Enemy::makeEnemyBody(int x, int y) {
	b2BodyDef bodyDef;
	bodyDef.position.Set((float32)x/10, (float32)y/10);
	bodyDef.type = b2_dynamicBody;
	return enemy_world->CreateBody(&bodyDef);
}

b2Shape* Enemy::makeEnemyShape() {
	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = (float32)(width / 20);
	return circle;
}

Enemy::~Enemy() {
	delete rect;
}

void Enemy::step(double seconds) {
	PhysicsParticle::step(seconds);

	//calculate new velocity
	Vector velocity(Point{ (int)x, (int)y }, player->getLoc());
	velocity = velocity.scaleTo(speed);

	//TODO: do this, but use another fixture and the collision engine to figure
	//out what is nearby
	/*
	// check for nearby enemies (again, replace this with spatial data structure later)
	Vector repel_force;
	double distance_squared;
	for (std::shared_ptr<Enemy> e : Enemy::all_enemies) {
		if (e.get() == this)
			continue;

		distance_squared = distanceSquared(Point{ x, y }, Point{ e->x, e->y });
		if (distance_squared < (max_affecting_distance * max_affecting_distance)) {
			//repel_force = repel_force + Vector(Point{ e->x, e->y }, Point{ x, y }).scaleTo(repel * repel / distance_squared);
			repel_force = repel_force + Vector(Point{ e->x, e->y }, Point{ x, y }).scaleTo(repel * (1 - sqrt(distance_squared) / max_affecting_distance));
		}
	}
	if (repel_force.getLength() > 0) {
		repel_force.scaleBy(1 / (all_enemies.size() - 1));

		if (repel_force.getLength() > speed * 2) {
			repel_force.scaleTo(speed * 2);
		}
		velocity = velocity + repel_force;
	}
	*/

	body->SetLinearVelocity(b2Vec2((float32)velocity.getX(), (float32)velocity.getY()));
}

void Enemy::draw(SDL_Renderer* ren) {
	//filledCircleColor(ren, x, y, enemy_width/2, 0xff0000ff);
	rect->x = (int)x - width / 2;
	rect->y = (int)y - height / 2;
	SDL_RenderCopy(ren, tex, NULL, rect);
}

void Enemy::hit(int damage) {
	hp -= damage;
}

EnemySpawn::EnemySpawn(int screenWidth, int screenHeight) :
	RingParticleSource( screenWidth / 2, screenHeight / 2,
		(int)sqrt(distanceSquared(Point{ screenWidth/2, screenHeight/2 }, Point{ 0, 0 })) + 50,
		10, true, true) {}

void EnemySpawn::generate_new_particles(int num) {
	int n = particles.size();
	RingParticleSource::generate_new_particles(num);
	/*
	size_t x = n;
	std::list<std::shared_ptr<Enemy>>::reverse_iterator it = particles.rbegin();
	while (x < particles.size()) {
		Enemy::all_enemies.push_back(*it);
		it++;
		x++;
	}
	*/
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
	{
		std::list<std::shared_ptr<Enemy>>::iterator it = particles.begin();
		while (it != particles.end()) {
			std::shared_ptr<Enemy> enemy = *it;
			enemy->step(seconds);
			if (enemy->is_dead()) {
				//create an explosion in the place of the dead enemy
				explosions.push_back(Explosion((int)enemy->x, (int)enemy->y));

				it = particles.erase(it);
			}
			else {
				it++;
			}
		}
	}

	//now step the explosions
	{
		std::list<Explosion>::iterator it = explosions.begin();
		while (it != explosions.end()) {
			Explosion& e = *it;
			if (e.is_over()) {
				it = explosions.erase(it);
			} else {
				e.step(seconds);
				it++;
			}
		}
	}
}

void EnemySpawn::draw_particles(SDL_Renderer* ren) {
	RingParticleSource::draw_particles(ren);

	std::list<Explosion>::iterator it = explosions.begin();
	while(it != explosions.end()) {
		(*it).draw_particles(ren);
		it++;
	}
}