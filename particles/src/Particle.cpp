#include "Particle.h"

Particle::Particle(int x, int y):
        x(x), y(y) { }

void Particle::step(double seconds) {
    //do nothing by default
}

std::shared_ptr<Particle> createParticleAt(int x, int y) {
	return std::shared_ptr<Particle>();
}
