#include "Particle.h"

#include <iostream>

namespace Particles {

	Particle::Particle(float x, float y) : p{ x, y } { }
	Particle::Particle(Point p) : p{ p.x, p.y } { }

	void Particle::step(double seconds) { /*do nothing by default*/ }

	std::shared_ptr<Particle> Particle::createParticleAt(float x, float y) { return std::shared_ptr<Particle>(); }

	PhysicsParticle::PhysicsParticle(float x, float y, b2World* world, b2Body* body, b2Shape* shape, int type,
		float32 density, float32 friction, float32 restitution, bool is_sensor) :
		Particle(x, y), world{ world }, body{ body }, shape{ shape }, type{ type } {
		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.userData = new PhysicsData{ type, this };
		fixtureDef.isSensor = is_sensor;
		fixture = body->CreateFixture(&fixtureDef);
	}

	PhysicsParticle::PhysicsParticle(Point p, b2World* world, b2Body* body, b2Shape* shape, int type,
		float32 density, float32 friction, float32 restitution, bool is_sensor) :
		PhysicsParticle{ p.x, p.y, world, body, shape, type, density, friction, restitution, is_sensor } {}

	PhysicsParticle::~PhysicsParticle() {
		world->DestroyBody(body);
		delete shape;
	}

	void PhysicsParticle::step(double seconds) {
		//get current location from physics body
		p.x = body->GetWorldCenter().x * 10;
		p.y = body->GetWorldCenter().y * 10;
	}

	TrajectoryParticle::TrajectoryParticle(float x, float y, double angle, int speed) :
		Particle(x, y), direction(Vector::getDir(angle).scaleTo(speed)), speed(speed) {}

	TrajectoryParticle::TrajectoryParticle(Point p, double angle, int speed) : TrajectoryParticle{p.x, p.y, angle, speed} {}

	void TrajectoryParticle::step(double seconds) {
		p.x += (float)(direction.getX() * seconds);
		p.y += (float)(direction.getY() * seconds);
	}

	PhysicsTrajectoryParticle::PhysicsTrajectoryParticle(float x, float y, double angle, int speed, b2World* world,
		b2Body* body, b2Shape* shape, int type, float32 density, float32 friction, float32 restitution, bool is_sensor) :
		PhysicsParticle(x, y, world, body, shape, type, density, friction, restitution, is_sensor), speed{ speed }, angle{ angle } {
		Vector direction = Vector::getDir(angle).scaleTo(speed);
		float32 vx = (float32)direction.getX();
		float32 vy = (float32)direction.getY();
		body->SetLinearVelocity(b2Vec2((float32)direction.getX(), (float32)direction.getY()));
	}

	PhysicsTrajectoryParticle::PhysicsTrajectoryParticle(Point p, double angle, int speed, b2World* world,
		b2Body* body, b2Shape* shape, int type, float32 density, float32 friction, float32 restitution, bool is_sensor) :
		PhysicsTrajectoryParticle(p.x, p.y, angle, speed, world, body, shape, type, density, friction, restitution, is_sensor) {}

	void PhysicsTrajectoryParticle::step(double seconds) { PhysicsParticle::step(seconds); }
}
