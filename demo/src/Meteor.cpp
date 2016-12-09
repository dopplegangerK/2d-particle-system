#include "Meteor.h"
#include <iostream>

b2World* Meteor::meteor_world = nullptr;
int Meteor::height = 0;
int Meteor::width = 0;
SDL_Texture* Meteor::tex = nullptr;

Meteor::Meteor(int x, int y, double angle, Point target) :
    PhysicsTrajectoryParticle(x, y, angle, speed, meteor_world, makeMeteorBody(x,y), makeMeteorShape(), 4, 5, 0, 1), target{target.x, target.y} {
        double rotation = (rand() / RAND_MAX) * (2 * PI);
        body->SetTransform(body->GetPosition(), rotation);
        int rotation = 1;
        if(rand() % 2 == 0)
            rotation = -1;
        body->ApplyAngularImpulse(10000 * rotation, true);
}

b2Body* Meteor::makeMeteorBody(int x, int y) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x/10.0f, y/10.0f);
    bodyDef.type = b2_dynamicBody;
    return meteor_world->CreateBody(&bodyDef);
}

b2Shape* Meteor::makeMeteorShape() {
    b2Vec2 vertices[6];
    vertices[0].Set(-6.4, -8.4);
    vertices[1].Set(4.4, -8.4);
    vertices[2].Set(10.0, -0.2);
    vertices[3].Set(6.8, 6.2);
    vertices[4].Set(-4.0, 8.4);
    vertices[5].Set(-10.0, 1.4);
    b2PolygonShape* pShape = new b2PolygonShape();
    pShape->Set(vertices, 6);
    return pShape;
}

void Meteor::setPhysicsWorld(b2World* world) { meteor_world = world; }

std::shared_ptr<Meteor> Meteor::createParticleAt(int x, int y) {
    Point p = getPointOnRing({ 512, 320 }, (int)distance(Point{ 512, 320 }, Point{ 0, 0 }) + 100);
    double angle = Vector({x, y}, p).getAngle();
    return std::make_shared<Meteor>(x, y, angle, p);
}

bool Meteor::is_dead() const {
    return distance({(int)x, (int)y}, target) <= 10;
}

void Meteor::draw(SDL_Renderer* ren) {
    SDL_Rect rect;
    rect.w = width;
    rect.h = height;
    rect.x = (int)x - rect.w / 2;
    rect.y = (int)y - rect.h / 2;
    SDL_RenderCopyEx(ren, tex, NULL, &rect, toDegrees(body->GetAngle()), NULL, SDL_FLIP_NONE);

    //draw collision shape's vertices, for debugging purposes
    b2Transform t = body->GetTransform();
    for (int k = 0; k < 6; k++) {
	b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(k);
	Point p = rotate({ (int)(vertex.x * 10), (int)(vertex.y * 10) }, t.q.GetAngle());
	filledCircleColor(ren, p.x + x, p.y + y, 3, 0xff00ffff);
    }
}

MeteorSpawn::MeteorSpawn(int screenWidth, int screenHeight) : 
    RingParticleSource( screenWidth / 2, screenHeight / 2,
    (int)distance(Point{ screenWidth/2, screenHeight/2 }, Point{ 0, 0 }) + 50,
    10, true, true) {}

void MeteorSpawn::step(double seconds) {
    RingParticleSource::step(seconds);
    time_to_spawn -= seconds;
    if(time_to_spawn <= 0) {
        generate_new_particles(1);
        time_to_spawn = (double)(rand() % (max_spawn_time - min_spawn_time) + min_spawn_time);
    }
}
