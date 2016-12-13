#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Vector.h"
#include <memory>
#include <SDL.h>
#include <Box2D/Box2D.h>

namespace Particles {

	class Particle {
	protected:
		Point p;
	public:
		Particle(float x, float y);
		Particle(Point p);
		virtual ~Particle() {}

		// NOTE: every implementable subclass of Particle should implement its own version of this method.
		// This one just returns a default shared_ptr
		static std::shared_ptr<Particle> createParticleAt(float x, float y);

		//draw the particle
		virtual void draw(SDL_Renderer* ren) = 0;
		//step the particle forward in time (does nothing by default)
		virtual void step(double seconds);
		//return true when the particle should die
		virtual bool isDead() const = 0;
	};

	// A pointer to a PhysicsData is stored in the fixture of each PhysicsParticle.
	struct PhysicsData {
		int type;	// store an enum or something to show what kind of object this is
					// (passed as the type argument to PhysicsParticle constructor)
		void* object; // pointer to this object so we can access its data when checking collisions or something
	};

	// Superclass for particles that are tied to the physics engine
	class PhysicsParticle : public Particle {
	protected:
		int type; // Used in PhysicsData to store info about this object in the b2Fixture
		b2World* world;
		b2Body* body;
		b2Shape* shape;
		b2Fixture* fixture;
	public:
		PhysicsParticle(float x, float y, b2World* world, b2Body* body, b2Shape* shape, int type = 0, float32 density = 0,
			float32 friction = 0, float32 restitution = 1, bool is_sensor = false);
		PhysicsParticle(Point p, b2World* world, b2Body* body, b2Shape* shape, int type = 0, float32 density = 0,
			float32 friction = 0, float32 restitution = 1, bool is_sensor = false);
		virtual ~PhysicsParticle();
		virtual void draw(SDL_Renderer* ren) = 0;
		virtual void step(double seconds);
		virtual bool isDead() const = 0;
	};

	// Particle that continues along a given trajectory with a given constant speed
	class TrajectoryParticle : public Particle {
	protected:
		Vector direction;
		int speed;
	public:
		TrajectoryParticle(float x, float y, double angle, int speed);
		TrajectoryParticle(Point p, double angle, int speed);
		virtual ~TrajectoryParticle() {}
		virtual void draw(SDL_Renderer* ren) = 0;
		virtual void step(double seconds);
		virtual bool isDead() const = 0;
	};

	// Same as above, but is also connected to the physics engine.
	// The particle is given an initial velocity and relies on the physics engine
	// to continue its movement.
	class PhysicsTrajectoryParticle : public PhysicsParticle {
	protected:
		int speed;
		double angle;
	public:
		PhysicsTrajectoryParticle(float x, float y, double angle, int speed, b2World* world,
			b2Body* body, b2Shape* shape, int type = 0, float32 density = 0, float32 friction = 0,
			float32 restitution = 1, bool is_sensor = false);
		PhysicsTrajectoryParticle(Point p, double angle, int speed, b2World* world,
			b2Body* body, b2Shape* shape, int type = 0, float32 density = 0, float32 friction = 0,
			float32 restitution = 1, bool is_sensor = false);
		virtual ~PhysicsTrajectoryParticle() {}
		virtual void draw(SDL_Renderer* ren) = 0;
		virtual void step(double seconds);
		virtual bool isDead() const = 0;
	};
}

#endif
