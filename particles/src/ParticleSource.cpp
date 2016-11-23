#include "ParticleSource.h"

template<class P>
ParticleSource<P>::ParticleSource(int density, bool isDynamic):
	density{ density }, isDynamic{ isDynamic } {
    initialize_particles();
}

template<class P>
void ParticleSource<P>::step(double seconds) {
    if(isDynamic) {
        for(P particle: particles) {
            particle.step(seconds);
        }
    }
}

template<class P>
void ParticleSource<P>::draw_particles() {
    for(P particle: particles) {
        particle.draw();
    }
}
