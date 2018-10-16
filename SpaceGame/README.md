# Space!

Space is a typical space shooter game in which the goal is to survive as long as possible and get as many points as possible.

## Controls:
- Move the mouse to steer (if you keep the mouse still, the ship will continue moving in that direction)
- Click to shoot
- ESC to open the pause menu

## Enemies:
Appearance | value  | hits to kill | description
-----------|--------|--------------|------------
Green      |  5 pts | 1 hit        | Will chase you until you kill them.
Red        | 20 pts | 2 hits       | These will mind their own business, until you shoot them once. Then they start shooting back.
Asteroids  |  0 pts | ? hits       | Instant death

##Use of Particles

For those interested in a demo of the Particles library, everything onscreen except the player's ship and the text is modeled as a particle.

##### Particle Types:
- Particle (the base class) - Stars
- PhysicsParticle - Enemies, asteroids
- TrajectoryParticle - Fire/explosion particles (see src/Explosion.cpp for how the explosions fade and change colors)
- PhysicsTrajectoryParticle - Bullets

##### Particle Sources:
- LineParticleSource - Fire (from the player's ship)
- PointParticleSource - Explosions
- RingParticleSource - Enemy and meteor spawns
- ScatteredParticleSource - Stars
