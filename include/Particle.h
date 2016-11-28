#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Vector.h"
#include <memory>
#include <SDL.h>
#include <Box2D/Box2D.h>

class Particle {
protected:
    double x;
    double y;

public:
    Particle(int x, int y);
	virtual ~Particle() {}

	// NOTE: every implaementable subclass of Particle should implement its own version of this method.
	// This one just returns a default unique_ptr
	static std::shared_ptr<Particle> createParticleAt(int x, int y);

    //draw the particle
    virtual void draw(SDL_Renderer* ren) = 0;
    //setp the particle forward in time
    //override unless the particle is stationary
    virtual void step(double seconds);
	virtual bool is_dead() const = 0;
};

struct PhysicsData {
	int type;	//store an enum or something
	void* object;
};

// Superclass for particles that are tied to the physics engine
class PhysicsParticle: public Particle {
protected:
	//used in PhysicsData to store info about this object in the b2Fixture
	int type;
	b2World* world;
	b2Body* body;
	b2Shape* shape;
	b2Fixture* fixture;
public:
	PhysicsParticle(int x, int y, b2World* world, b2Body* body, b2Shape* shape, int type = 0, float32 density = 0, float32 friction = 0, float32 restitution = 1, bool isSensor = false);
	virtual ~PhysicsParticle();
	virtual void step(double seconds);
};

// Particle that continues along a given trajectory with a given constant speed
class TrajectoryParticle : public Particle {
protected:
	Vector direction;
	int speed;
public:
	TrajectoryParticle(int x, int y, double angle, int speed);
	virtual ~TrajectoryParticle() {}
	virtual void step(double seconds);
};

// Same as above, but is also connected to the physics engine.
// The particle is given an initial velocity and relies on the physics engine
// to continue its movement.
class PhysicsTrajectoryParticle : public PhysicsParticle {
protected:
	int speed;
	double angle;
public:
	PhysicsTrajectoryParticle(int x, int y, double angle, int speed, b2World* world,
		b2Body* body, b2Shape* shape, int type = 0, float32 density = 0, float32 friction = 0,
		float32 restitution = 1, bool isSensor = false);
	virtual ~PhysicsTrajectoryParticle() {}
};

#endif
