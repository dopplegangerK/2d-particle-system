#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <memory>
#include <SDL.h>

class Particle {
protected:
    int x;
    int y;

public:
    Particle(int x, int y);

	// NOTE: every implaementable subclass of Particle should implement its own version of this method.
	// This one just returns a default unique_ptr
	static std::shared_ptr<Particle> createParticleAt(int x, int y);

    //draw the particle
    virtual void draw(SDL_Renderer* ren) = 0;
    //setp the particle forward in time
    //override unless the particle is stationary
    virtual void step(double seconds);
	virtual bool is_dead() = 0;
};

#endif
