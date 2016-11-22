#include Particle.h

Particle::Particle(int x, int y, double lifetime):
        x(x), y(y), lifetime(lifetime) { }

void Particle::step(double seconds) {
    //do nothing by default
}
