#include ParticleSource.h

ParticleSource::ParticleSource(int density, bool isDynamic):
        densite(density), isDynamic(isDynamic) {
    initialize_particles();
}

void ParticleSource::step(double seconds) {
    if(isDynamic) {
        for(P particle: particles) {
            particle.step(seconds);
        }
    }
}

void ParticleSource::draw_particles() {
    for(P particle: particles) {
        particle.draw();
    }
}
