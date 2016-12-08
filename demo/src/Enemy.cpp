#include "Enemy.h"
#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

Rocket* Enemy::player = nullptr;
b2World* Enemy::enemy_world = nullptr;

Enemy::Enemy(int x, int y, int w, int h, SDL_Texture* tex, int hp) :
		PhysicsParticle(x, y, enemy_world, makeEnemyBody(x, y), makeEnemyShape(w/2), 1, 1),
		direction{ 0, 0 }, myTex{ tex }, hp { hp } {
	speed = rand() % (max_speed - min_speed) + min_speed;
	rect.w = w;
	rect.h = h;

	Vector velocity(Point{ (int)x, (int)y }, player->getLoc());
	velocity = velocity.scaleTo(speed);
	body->SetLinearVelocity(b2Vec2((float32)velocity.getX(), (float32)velocity.getY()));

	b2Filter filter;
	filter.categoryBits = 0x0008;
	fixture->SetFilterData(filter);
}

b2Body* Enemy::makeEnemyBody(int x, int y) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x/10.0f, y/10.0f);
	bodyDef.type = b2_dynamicBody;
	return enemy_world->CreateBody(&bodyDef);
}

b2Shape* Enemy::makeEnemyShape(int r) {
	b2CircleShape* circle = new b2CircleShape();
	circle->m_radius = (float32)(r / 20.0);
	return circle;
}

void Enemy::draw(SDL_Renderer* ren) {
	//filledCircleColor(ren, x, y, enemy_width/2, 0xff0000ff);
	rect.x = (int)x - rect.w / 2;
	rect.y = (int)y - rect.h / 2;
	SDL_RenderCopy(ren, myTex, NULL, &rect);
}

void Enemy::hit(int damage) {
	hp -= damage;
	Explosion::play_sound();
}

std::shared_ptr<Enemy> Enemy::createParticleAt(int x, int y) {
	int color_choice = rand() % 4;
	if (color_choice < 3)
		return std::make_shared<GreenEnemy>(x, y);
	else
		return std::make_shared<RedEnemy>(x, y);
}

/***************
 * Green Enemy *
 ***************/

int GreenEnemy::height = 0;
int GreenEnemy::width = 0;
SDL_Texture* GreenEnemy::tex = nullptr;

GreenEnemy::GreenEnemy(int x, int y) : Enemy(x, y, width, height, tex, 1) {}

void GreenEnemy::step(double seconds) {
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

/*************
 * Red Enemy *
 *************/

int RedEnemy::height = 0;
int RedEnemy::width = 0;
SDL_Texture* RedEnemy::tex = nullptr;

RedEnemy::RedEnemy(int x, int y) : Enemy(x, y, height, width, tex, 2),
	target(getPointOnRing({ 512, 320 }, (int)distance(Point{ 512, 320 }, Point{ 0, 0 }) + 50)) {}

void RedEnemy::shoot() {
	if (gun == nullptr) {
		gun = std::make_shared<BulletSource<EnemyBullet>>(x, y);
	} else {
		gun->moveTo(x, y);
	}
	gun->fire(Vector({ (int)x, (int)y }, player->getLoc()).getAngle());
}

void RedEnemy::step(double seconds) {
	PhysicsParticle::step(seconds);

	//calculate new velocity
	Vector velocity = Vector(Point{ (int)x, (int)y }, target).scaleTo(speed);
	body->SetLinearVelocity(b2Vec2((float32)velocity.getX(), (float32)velocity.getY()));

	if (hp < 2) {
		if (shot_time <= 0) {
			shoot();
			shot_time = max_shot_time;
		}
		shot_time -= seconds;
	}
	if (gun != nullptr)
		gun->step(seconds);
}

bool RedEnemy::is_dead() const {
	return Enemy::is_dead() || distance(target, Point{ (int)x, (int)y }) <= 5;
}

void RedEnemy::draw(SDL_Renderer* ren) {
	Enemy::draw(ren);
	if(gun != nullptr)
		gun->draw_particles(ren);
}

/***************
 * Enemy Spawn *
 ***************/

EnemySpawn::EnemySpawn(int screenWidth, int screenHeight) :
	RingParticleSource( screenWidth / 2, screenHeight / 2,
		(int)distance(Point{ screenWidth/2, screenHeight/2 }, Point{ 0, 0 }) + 50,
		10, true, true) {}

void EnemySpawn::generate_new_particles(int num) {
	int n = particles.size();
	RingParticleSource::generate_new_particles(num);
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
	step_explosions(seconds);
}


void EnemySpawn::step_explosions(double seconds) {
	std::list<Explosion>::iterator it = explosions.begin();
	while (it != explosions.end()) {
		Explosion& e = *it;
		if (e.is_over()) {
			it = explosions.erase(it);
		}
		else {
			e.step(seconds);
			it++;
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

void EnemySpawn::clear() {
	particles.clear();
	explosions.clear();
}
