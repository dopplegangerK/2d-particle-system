#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <memory>
#include <SDL.h>
#include <Box2D/Box2D.h>

class Particle {
protected:
    int x;
    int y;

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

class PhysicsParticle: public Particle {
protected:
	//used in PhysicsData to store info about this object in the b2Fixture
	int type;
	b2World* world;
	b2Body* body;
	b2Shape* shape;
public:
	PhysicsParticle(int x, int y, b2World* world, b2Body* body, b2Shape* shape, int type = 0, float32 density = 0, float32 friction = 0, float32 restitution = 1);
	virtual ~PhysicsParticle();
};

#endif
