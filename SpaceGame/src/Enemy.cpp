#include "Enemy.h"
#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

Rocket* Enemy::player = nullptr;
b2World* Enemy::enemy_world = nullptr;

Enemy::Enemy(float x, float y, int w, int h, SDL_Texture* tex, int hp) :
		PhysicsParticle(x, y, enemy_world, makeEnemyBody((int)x, (int)y), makeEnemyShape(w/2), 1, 1),
		direction{ 0, 0 }, myTex{ tex }, hp { hp } {
	speed = rand() % (max_speed - min_speed) + min_speed;
	rect.w = w;
	rect.h = h;

	Vector velocity(p, player->getLoc());
	velocity = velocity.scaleTo(speed);
	body->SetLinearVelocity(b2Vec2((float32)velocity.getX(), (float32)velocity.getY()));

	// Set filter category so we don't collide with our own bullets
	b2Filter filter;
	filter.categoryBits = collision_category;
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
	circle->m_radius = (float32)(r / 10.0);
	return circle;
}

void Enemy::draw(SDL_Renderer* ren) {
	rect.x = (int)p.x - rect.w / 2;
	rect.y = (int)p.y - rect.h / 2;
	SDL_RenderCopy(ren, myTex, NULL, &rect);
}

void Enemy::hit(int damage) {
	hp -= damage;
	if(hp <= 0)
		Explosion::playSound();
}

std::shared_ptr<Enemy> Enemy::createParticleAt(float x, float y) {
	int color_choice = rand() % 3;
	if (color_choice < 2)
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

GreenEnemy::GreenEnemy(float x, float y) : Enemy(x, y, width, height, tex, 1) {}

void GreenEnemy::step(double seconds) {
	PhysicsParticle::step(seconds);
	Vector velocity = Vector(p, player->getLoc()).scaleTo(speed);
	body->SetLinearVelocity(b2Vec2((float32)velocity.getX(), (float32)velocity.getY()));
}

/*************
 * Red Enemy *
 *************/

int RedEnemy::height = 0;
int RedEnemy::width = 0;
SDL_Texture* RedEnemy::tex = nullptr;

RedEnemy::RedEnemy(float x, float y) : Enemy(x, y, height, width, tex, 2),
	target(getPointOnRing({ 512, 320 }, (float)distance(Point{ 512, 320 }, Point{ 0, 0 }) + 50)) {}

void RedEnemy::shoot() {
	if (gun == nullptr) {
		gun = std::make_shared<BulletSource<EnemyBullet>>(p);
	} else {
		gun->moveTo(p.x, p.y);
	}
	gun->fire(Vector(p, player->getLoc()).getAngle());
}

void RedEnemy::step(double seconds) {
	PhysicsParticle::step(seconds);

	//calculate new velocity
	Vector velocity = Vector(p, target).scaleTo(speed);
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

bool RedEnemy::isDead() const {
	return Enemy::isDead() || distance(target, p) <= 5;
}

void RedEnemy::draw(SDL_Renderer* ren) {
	if(gun != nullptr)
		gun->drawParticles(ren);
	Enemy::draw(ren);
}

/***************
 * Enemy Spawn *
 ***************/

EnemySpawn::EnemySpawn(int screenWidth, int screenHeight) :
	RingParticleSource(Point{ screenWidth / 2.0f, screenHeight / 2.0f },
		(float)distance(Point{ screenWidth/2.0f, screenHeight/2.0f }, Point{ 0.0f, 0.0f }) + 50,
		10, true, true) {}

void EnemySpawn::generateNewParticles(int num) {
	int n = particles.size();
	RingParticleSource::generateNewParticles(num);
}

void EnemySpawn::initializeParticles() {
	generateNewParticles(1);
}

void EnemySpawn::step(double seconds) {
	//spawn new enemy if we've passed the given timestep
	time += seconds;
	if (time >= time_to_spawn) {
		time = 0;
		generateNewParticles(1);
	}
        if(level < 10) {
            level_time += seconds;
            if(level_time >= level_length) {
                level_time = 0;
                level++;
                time_to_spawn *= difficulty_coeff;
            }
        }

	//step all enemies
	{
		std::list<std::shared_ptr<Enemy>>::iterator it = particles.begin();
		while (it != particles.end()) {
			std::shared_ptr<Enemy> enemy = *it;
			enemy->step(seconds);
			if (enemy->isDead()) {
				//create an explosion in the place of the dead enemy
				explosions.push_back(Explosion(enemy->p));
				it = particles.erase(it);
			}
			else {
				it++;
			}
		}
	}

	//now step the explosions
	stepExplosions(seconds);
}


void EnemySpawn::stepExplosions(double seconds) {
	std::list<Explosion>::iterator it = explosions.begin();
	while (it != explosions.end()) {
		Explosion& e = *it;
		if (e.isOver()) {
			it = explosions.erase(it);
		}
		else {
			e.step(seconds);
			it++;
		}
	}
}

void EnemySpawn::drawParticles(SDL_Renderer* ren) {
	RingParticleSource::drawParticles(ren);

	std::list<Explosion>::iterator it = explosions.begin();
	while(it != explosions.end()) {
		(*it).drawParticles(ren);
		it++;
	}
}

void EnemySpawn::clear() {
    level = 0;
    level_time = 0;
    time_to_spawn = 1.5;
	particles.clear();
	explosions.clear();
}
