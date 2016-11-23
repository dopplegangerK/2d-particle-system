#ifndef _PARTICLE_H_
#define _PARTICLE_H_

class Particle {
protected:
    int x;
    int y;
    double time_lived;
    double lifetime;

    static double constexpr FOREVER = -1;
public:
    Particle(int x, int y, double lifetime);

    //draw the particle
    virtual void draw() = 0;
    //setp the particle forward in time
    //override unless the particle is stationary
    virtual void step(double seconds);
};

#endif
