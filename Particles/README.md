#Particles

This library is intended to be adapted for a wide variety of uses, and as such, does not code for any specific types of particles. It is intended to be used as a framework. To see how these particles can be implemented, see their uses in the SpaceGame directory.

To use, include the header `Particles.h`

##The Particle class
To create a particle type, you must first override the `Particle` class.

#####Methods that must be overridden:
- `void draw(SDL_Renderer* ren)` - draw the given particle at it's location (saved as a Point struct)
- `bool isDead()` - return a bool stating whether or not this particle should be removed from the simulation
- `void step(double seconds)` - step the particle simulation by the given amount. While this is not required to be overridden, the default action is to do nothing, which is usually not intended behavior.
- Each particle must also implement a factory constructor: `static std::shared_ptr<P> createParticleAt(float x, float y)`, where P is the name of the class.

##The ParticleSource class
The `ParticleSource` class is responsible for creating, managing, and destroying particles.

#####Useful methods:
- `void generateParticles(int num)` - create this number of particles. This is purely virtual in the base class.
- `void initializeParticles()` - Initialize the number of particles that was passed as `density` to the base class's constructor. Note that particles will not be initialized by default.
- `void step(double seconds)` - Step all particles by this amount. By default, it just loops through its particles calling their `step()` methods.
- `void drawParticles(SDL_Renderer* ren)` - Draw all your particles. Again, the default just loops through and calls each particle's `draw()` method.
- `void clear()` - delete all particles

In addition, the library provides some useful extensions of the `ParticleSource` class:
#####Provided ParticleSource types:
- `LineParticleSource` - Takes in a start point and end point and spawns particles at random along that line
- `PointParticleSource` - Spawn particles at some central point
- `RingParticleSource` - Spawn particles at random along the edge of a circle
- `ScatteredParticleSource` - Spawn particles at random within a given rectangle

See those classes' header files for more information on how to use them.
