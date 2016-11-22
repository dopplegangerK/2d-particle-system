#ifndef _PARTICLE_SOURCE_H_
#define _PARTICLE_SOURCE_H_

template<class P>
class ParticleSource {
protected:
    std::vector<P> particles;
    int density;
    bool isDynamic;

    //set the position and state of all particles
    virtual void initialize_particles() = 0;
    //add more particles
    virtual void generate_particles(int num) = 0;
public:
    ParticleSource(int density, bool isDynamic = true);
    virtual ~ParticleSource();
    virtual void step(double seconds);
    virtual void draw_particles();
}

#endif
