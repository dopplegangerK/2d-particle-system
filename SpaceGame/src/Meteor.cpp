#include "Meteor.h"

b2World* Meteor::meteor_world = nullptr;
int Meteor::height = 0;
int Meteor::width = 0;
SDL_Texture* Meteor::tex = nullptr;
Mix_Chunk* Meteor::sound = nullptr;

Meteor::Meteor(float x, float y, double angle, Point target) :
    PhysicsTrajectoryParticle(x, y, angle, speed, meteor_world, makeMeteorBody(x,y), makeMeteorShape(), 4, 5, 0, 1), target{target.x, target.y} {
	float32 rotation = (float32)((rand() / RAND_MAX) * (2 * PI));
	body->SetTransform(body->GetPosition(), rotation);
	int clockwise = 1;
	if(rand() % 2 == 0)
		clockwise = -1;
	body->ApplyAngularImpulse(10000.0f * clockwise, true);
	
	channel = Mix_FadeInChannel(-1, sound, 0, 1000);
}

Meteor::~Meteor() {
	Mix_FadeOutChannel(channel, 1000);
}

b2Body* Meteor::makeMeteorBody(float x, float y) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x/10.0f, y/10.0f);
    bodyDef.type = b2_dynamicBody;
    return meteor_world->CreateBody(&bodyDef);
}

b2Shape* Meteor::makeMeteorShape() {
    b2Vec2 vertices[6];
    vertices[0].Set(-6.4f, -8.4f);
    vertices[1].Set(4.4f, -8.4f);
    vertices[2].Set(10.0f, -0.2f);
    vertices[3].Set(6.8f, 6.2f);
    vertices[4].Set(-4.0f, 8.4f);
    vertices[5].Set(-10.0f, 1.4f);
    b2PolygonShape* pShape = new b2PolygonShape();
    pShape->Set(vertices, 6);
    return pShape;
}

void Meteor::setPhysicsWorld(b2World* world) { meteor_world = world; }

std::shared_ptr<Meteor> Meteor::createParticleAt(float x, float y) {
    Point p = getPointOnRing({ 512.0f, 320.0f }, (float)distance(Point{ 512.0f, 320.0f }, Point{ 0.0f, 0.0f }) + 100);
    double angle = Vector({x, y}, p).getAngle();
    return std::make_shared<Meteor>(x, y, angle, p);
}

bool Meteor::isDead() const {
    return distance(p, target) <= 10;
}

void Meteor::draw(SDL_Renderer* ren) {
    SDL_Rect rect;
    rect.w = width;
    rect.h = height;
    rect.x = (int)p.x - rect.w / 2;
    rect.y = (int)p.y - rect.h / 2;
    SDL_RenderCopyEx(ren, tex, NULL, &rect, toDegrees(body->GetAngle()), NULL, SDL_FLIP_NONE);

    /*
    //draw collision shape's vertices, for debugging purposes
    b2Transform t = body->GetTransform();
    for (int k = 0; k < 6; k++) {
	b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(k);
	Point p = rotate({ (int)(vertex.x * 10), (int)(vertex.y * 10) }, t.q.GetAngle());
	filledCircleColor(ren, p.x + x, p.y + y, 3, 0xff00ffff);
    }
    */
}

MeteorSpawn::MeteorSpawn(int screenWidth, int screenHeight) : 
    RingParticleSource( screenWidth / 2.0f, screenHeight / 2.0f,
    (float)distance(Point{ screenWidth / 2.0f, screenHeight / 2.0f }, Point{ 0.0f, 0.0f }) + 50,
    10, true, true) {}

void MeteorSpawn::step(double seconds) {
    RingParticleSource::step(seconds);
    time_to_spawn -= seconds;
    if(time_to_spawn <= 0) {
        generateNewParticles(1);
        time_to_spawn = (double)(rand() % (max_spawn_time - min_spawn_time) + min_spawn_time);
    }
}
