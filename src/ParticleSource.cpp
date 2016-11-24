#include "ParticleSource.h"
/*
template<class P>
ParticleSource<P>::ParticleSource(int density, bool isDynamic, bool constantStream):
	density{ density }, dynamic{ isDynamic }, constant{ constantStream } { }

template <class P>
void ParticleSource<P>::initialize_particles() {
	generate_new_particles(density);
}

template<class P>
void ParticleSource<P>::step(double seconds) {
    if(dynamic && particles.size() > 0) {
		int dead = 0;
		std::list<P>::iterator it = particles.begin();
		while (it != particles.end()) {
			std::shared_ptr<P> particle = particles.at(it);
			particle->step(seconds);
			if (particle->is_dead()) {
				std::list<P>::iterator dead = it;
				it++;
				list.erase(dead);
				dead++;
			}
			else {
				it++;
			}
		}
		if (dead > 0)
			generate_new_particles(dead);
    }
}

template<class P>
void ParticleSource<P>::draw_particles(SDL_Renderer* ren) {
    for(P particle: particles) {
        particle->draw(ren);
    }
}
*/
