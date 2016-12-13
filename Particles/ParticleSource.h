#ifndef _PARTICLE_SOURCE_H_
#define _PARTICLE_SOURCE_H_

#include <list>
#include <memory>
#include <SDL.h>

namespace Particles {

	template<class P>
	class ParticleSource {
	protected:
		std::list<std::shared_ptr<P>> particles;
		// Approximate number of particles that should be onscreen at one time
		int density;
		// Do these particles move? (Do we need to step them every time?)
		bool dynamic;
		// Do new particles spawn after old ones die? Is there a constant stream?
		bool constant;

		//add more particles
		virtual void generateNewParticles(int num) = 0;
	public:
		ParticleSource(int density, bool dynamic = true, bool constant = false);
		virtual ~ParticleSource() {}

		// Set the position and state of all particles.
		// This must be called by the user's code before using the particle source.
		// By default, it just calls generateNewParticles(density)
		virtual void initializeParticles();
		virtual void step(double seconds);
		virtual void drawParticles(SDL_Renderer* ren);
		virtual void clear();
	};

	template<class P>
	ParticleSource<P>::ParticleSource(int density, bool isDynamic, bool constantStream) :
		density{ density }, dynamic{ isDynamic }, constant{ constantStream } { }

	template <class P>
	void ParticleSource<P>::initializeParticles() {
		generateNewParticles(density);
	}

	template<class P>
	void ParticleSource<P>::step(double seconds) {
		if (dynamic && particles.size() > 0) {
			int dead = 0;
			typename std::list<std::shared_ptr<P>>::iterator it = particles.begin();
			while (it != particles.end()) {
				std::shared_ptr<P> particle = *it;
				particle->step(seconds);
				if (particle->isDead()) {
					it = particles.erase(it);
					dead++;
				}
				else {
					it++;
				}
			}
			if (constant && dead > 0)
				generateNewParticles(dead);
		}
	}

	template<class P>
	void ParticleSource<P>::drawParticles(SDL_Renderer* ren) {
		for (std::shared_ptr<P> particle : particles) {
			particle->draw(ren);
		}
	}

	template<class P>
	void ParticleSource<P>::clear() {
		particles.clear();
	}
}

#endif
