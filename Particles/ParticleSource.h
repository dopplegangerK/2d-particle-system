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
	// This must be called by the user's code before using the particle source.
	virtual void initialize_particles();
    virtual void step(double seconds);
    virtual void draw_particles(SDL_Renderer* ren);
};

template<class P>
ParticleSource<P>::ParticleSource(int density, bool isDynamic, bool constantStream) :
	density{ density }, dynamic{ isDynamic }, constant{ constantStream } { }

template <class P>
void ParticleSource<P>::initialize_particles() {
	generate_new_particles(density);
}

template<class P>
void ParticleSource<P>::step(double seconds) {
	if (dynamic && particles.size() > 0) {
		int dead = 0;
		std::list<std::shared_ptr<P>>::iterator it = particles.begin();
		while (it != particles.end()) {
			std::shared_ptr<P> particle = *it;
			particle->step(seconds);
			if (particle->is_dead()) {
				it = particles.erase(it);
				dead++;
			}
			else {
				it++;
			}
		}
		if (constant && dead > 0)
			generate_new_particles(dead);
	}
}

template<class P>
void ParticleSource<P>::draw_particles(SDL_Renderer* ren) {
	for (std::shared_ptr<P> particle : particles) {
		particle->draw(ren);
	}
}

#endif
