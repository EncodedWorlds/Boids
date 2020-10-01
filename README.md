# Boids

Boid nesting grounds project. Showing off the emergent flocking behavior from the application and interaction of simple steering forces. Watch these bird-like objects soar through the sky, avoid obstacles, and find other flockmates to fly with!

[Video of Boid flocking in action](https://youtu.be/FRgidMODDaI)  

## Project Features
Contains:
* Boid class  
An autonomous actor that can be spawned into the level and exhibit a bird-like, flocking motion with other Boid actors.  

* Flock Manager class  
Actor placed in the level that stores the perception and steering settings of the boids it controls. Used as a way to manipulate the behavior of the entire flock and optimize flock-wide logic changes.  

* Boid Cage Spawner  
An actor that can be placed in the world to spawn and contain Boids in a designated area. Boids that leave the cage boundary are teleported to the other side, similar to the game Asteroids.  

* Point Spawner  
An actor you can place in the world and spawn a number of Boids from its location that go in random directions.  

* Volume Spawner  
An actor you can place in the world that spawns a set number of Boids in a Volume and headed in the direction of the flow Arrow. Allows the emulation of small burst of Boids exiting a finite space (i.e. nest) or a continuous flow of Boids similar to an enormous flock leaving a cave or directional migration over vast area.  

* Volume Despawner  
An actor that can be placed in the world that despawns Boids that enter it. Used with attraction forces from BoidTargetObjects to "pull" Boids into it and emulate flock leaving the world (i.e. entering nest, exiting migration area, etc.).  

* Target Object  
An actor that can be placed in the world to attract/repel Boids by applying steering forces on all Boids within its range.  

* Nesting Grounds Level  
A tutorial level demonstrating how the systems work. Tweak the flock settings, add obstacles, or modify assets to see how the flock's behavior changes.

## Project Details
Engine: Unreal Engine 4  
Version: 4.25  
Language: C++  

## References
Boid behavior based off research done by Craig Reynolds.\
https://www.red3d.com/cwr/boids/  
Steering behaviors for autonomous characters paper.\
https://www.red3d.com/cwr/steer/gdc99/  

This project also was inspirationally fueled from a YouTube talk by Daniel Shiffman.\
Coding Challenge #124: Flocking Simulation.\
https://youtu.be/mhjuuHl6qHM  
p5js code source and interactive example.  
https://github.com/CodingTrain/website/blob/master/CodingChallenges/CC_124_Flocking_Boids/P5/boid.js  
https://processing.org/examples/flocking.html  

Collision avoidance system inspired by YouTube demo of Sebastian Lague.\
Coding Adventure: Boids.\
https://youtu.be/bqtqltqcQhw  
Source (C# in Unity).\
https://github.com/SebLague/Boids  

Spherical raycast avoidance design from Stackoverflow discussion.\
https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere/44164075#44164075  
