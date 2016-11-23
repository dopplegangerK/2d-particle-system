#ifndef _PARTICLE_SOURCE_H_
#define _PARTICLE_SOURCE_H_

#include <list>
#include <memory>
#include <SDL.h>

template<class P>
class ParticleSource {
protected:
    std::list<std::shared_ptr<P>> particles;
    int density;
    bool dynamic;
	bool constant;

    //add more particles
    virtual void generate_new_particles(int num) = 0;
public:
    ParticleSource(int density, bool dynamic = true, bool constant = false);
	virtual ~ParticleSource() {}
	// Set the position and state of all particles.
	// This must be called before using the particle source.
	virtual void initialize_particles();
    virtual void step(double seconds);
    virtual void draw_particles(SDL_Renderer* ren);
};

#endif
