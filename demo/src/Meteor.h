#ifndef _METEOR_H_
#define _METEOR_H_

#include <Particles.h>
#include <Box2D/Box2D.h>
#include <SDL_mixer.h>

using namespace Particles;

//type = 4
class Meteor : public PhysicsTrajectoryParticle {
private:
    static constexpr int speed = 30;

    static b2World* meteor_world;

    Point target;

    int channel;
public:
    static constexpr char* sprite = "../../demo/sprites/meteorGrey_big1.png";
    static SDL_Texture* tex;
    static int width;
    static int height;

    static constexpr char* sound_path = "../../demo/sounds/rumble2.wav";
    static Mix_Chunk* sound;

    static void setPhysicsWorld(b2World* world);

    static b2Body* makeMeteorBody(float x, float y);
    static b2Shape* makeMeteorShape();

    static std::shared_ptr<Meteor> createParticleAt(float x, float y);

    Meteor(float x, float y, double angle, Point p);
    virtual ~Meteor();

    virtual bool isDead() const;
    virtual void draw(SDL_Renderer* ren);
};

class MeteorSpawn : public RingParticleSource<Meteor> {
private:
    static constexpr int max_spawn_time = 20;
    static constexpr int min_spawn_time = 5;

    double time_to_spawn;
public:
    MeteorSpawn(int screenWidth, int screenHeight);
    virtual void step(double seconds);
};

#endif
