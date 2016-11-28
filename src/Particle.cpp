#include "Particle.h"

#include <iostream>

Particle::Particle(int x, int y):
        x(x), y(y) { }

void Particle::step(double seconds) {
    //do nothing by default
}

std::shared_ptr<Particle> createParticleAt(int x, int y) {
	return std::shared_ptr<Particle>();
}

PhysicsParticle::PhysicsParticle(int x, int y, b2World* world, b2Body* body, b2Shape* shape, int type, float32 density, float32 friction, float32 restitution, bool isSensor) :
	Particle(x, y), world{ world }, body{ body }, shape{ shape }, type{ type } {
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.userData = new PhysicsData{ type, this };
	fixtureDef.isSensor = isSensor;
	fixture = body->CreateFixture(&fixtureDef);
}

PhysicsParticle::~PhysicsParticle() {
	world->DestroyBody(body);
	delete shape;
}

void PhysicsParticle::step(double seconds) {
	//get current location from physics body
	x = (int)(body->GetWorldCenter().x * 10);
	y = (int)(body->GetWorldCenter().y * 10);
}

TrajectoryParticle::TrajectoryParticle(int x, int y, double angle, int speed) :
	Particle(x, y), direction(Vector::getDir(angle).scaleTo(speed)), speed(speed) {}

void TrajectoryParticle::step(double seconds) {
	x += direction.getX() * seconds;
	y += direction.getY() * seconds;
}

PhysicsTrajectoryParticle::PhysicsTrajectoryParticle(int x, int y, double angle, int speed, b2World* world,
		b2Body* body, b2Shape* shape, int type, float32 density, float32 friction, float32 restitution, bool isSensor) :
		PhysicsParticle(x, y, world, body, shape, type, density, friction, restitution, isSensor), speed{ speed }, angle{ angle } {
	Vector direction = Vector::getDir(angle).scaleTo(speed);
	float32 vx = (float32)direction.getX();
	float32 vy = (float32)direction.getY();
	body->SetLinearVelocity(b2Vec2((float32)direction.getX(), (float32)direction.getY()));
}
