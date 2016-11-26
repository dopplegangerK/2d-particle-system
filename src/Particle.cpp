#include "Particle.h"

Particle::Particle(int x, int y):
        x(x), y(y) { }

void Particle::step(double seconds) {
    //do nothing by default
}

std::shared_ptr<Particle> createParticleAt(int x, int y) {
	return std::shared_ptr<Particle>();
}

PhysicsParticle::PhysicsParticle(int x, int y, b2World* world, b2Body* body, b2Shape* shape, int type, float32 density, float32 friction, float32 restitution) :
	Particle(x, y), world{ world }, body{ body }, shape{ shape }, type{ type } {
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.userData = new PhysicsData{ type, this };
	body->CreateFixture(&fixtureDef);
}

PhysicsParticle::~PhysicsParticle() {
	world->DestroyBody(body);
	delete shape;
}
