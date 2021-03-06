// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//An autonomous actor that can be spawned into the level and exhibit a bird-like, flocking motion with other Boid actors.
//
//The Boid mechanics are broken into 3 levels; Action, Steering, Locomotion.
//Action is the goal for the Boid, this implementation uses "flocking" but it could also be objectives like "flee", "hunt", "wander", etc.
//Steering layer determines the path the Boid will take. This implementation uses perception to apply flocking forces to the Boid, but it could be boiled down to something as easy as "turn left" (i.e. a rider telling a horse to move left or giddy-up).
//Locomotion layer takes the inputs from steering and applies the physical attributes of the Boid to implement the desired movement. The Boid uses its engines, brakes, wings, legs, wheels, rudder, etc. to physically attempt to follow the path indicated by the steering inputs.

//boid behavior based off research done by Craig Reynolds
//https://www.red3d.com/cwr/boids/
//steering behaviors for autonomous characters paper
//https://www.red3d.com/cwr/steer/gdc99/

//this project also was inspirationally fueled from a YouTube talk by Daniel Shiffman
//Coding Challenge #124: Flocking Simulation
//https://youtu.be/mhjuuHl6qHM
//p5js code source and interactive example
//https://github.com/CodingTrain/website/blob/master/CodingChallenges/CC_124_Flocking_Boids/P5/boid.js
//https://processing.org/examples/flocking.html

//collision avoidance system inspired by YouTube demo of Sebastian Lague
//Coding Adventure: Boids
//https://youtu.be/bqtqltqcQhw
//source (C# in Unity)
//https://github.com/SebLague/Boids

//spherical raycast avoidance design from Stackoverflow discussion
//https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere/44164075#44164075
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

//forward declares
class UStaticMeshComponent;
class USphereComponent;
class AFlockManager;

UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()

public:
	//default constructor
	ABoid();

	//called to update each frame
	virtual void Tick(float DeltaTime) override;

protected:
	//called on level start or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
protected:
	//boid collision component
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	USphereComponent* BoidCollision;

	//boid static mesh component for visuals
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UStaticMeshComponent* BoidMesh;

	//collision sensor used to generate cohesion steering forces
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	USphereComponent* PerceptionSensor;

	//FLOCK MANAGER
protected:
	//flock manager that controls this boid's perception and steering behaviors
	AFlockManager* FlockManager;

public:
	inline AFlockManager* GetFlockManager() { return FlockManager; }

	//TODO: create "FindFlockManager" helper function to look for a flock manager for cases where they don't get properly assigned (i.e. a manually placed boid in editor).

	//MOVEMENT
protected:
	//boid velocity
	FVector BoidVelocity;

	//boid mesh rotation used for smoothly interpolating mesh rotations
	FRotator CurrentRotation;

	//smoothly updates the boid mesh's rotation to follow the velocity
	void UpdateMeshRotation();

	//TODO: add physical parameters to boid motion, mass, turning radius, max acceleration/braking force, gravity, etc.

public:
	UFUNCTION(BlueprintCallable)
	inline FVector GetBoidVelocity() { return BoidVelocity; };

	//STEERING
protected:
	//return separation steering force directed to avoid crowding/collision with local flockmates
	FVector	Separate(TArray<AActor*> Flock);
	//return alignment steering force directed towards the average heading of local flockmates
	FVector Align(TArray<AActor*> Flock);
	//return cohesion steering force directed toward the average position of local flockmates
	FVector GroupUp(TArray<AActor*> Flock);
	//apply behavioral steering to boid and update movement
	void Steer(float DeltaTime);

	//AVOIDANCE
protected:
	//checks if boid is on imminent collision course with obstacle
	bool IsObstacleAhead();
	//return obstacle avoidance force steering towards the unobstructed direction
	FVector AvoidObstacle();

	//TARGET STEERING
protected:
	//list of target forces
	TArray<FVector> TargetForces;

public:
	//adds target force to stack to be applied
	void AddTargetForce(FVector TargetForce);

	//DEBUG
	//TODO: add debug mode for showing boid collision, perception radius, velocity, sensors, etc.

	//ACTION
	//TODO: create goal setting logic (simple state machine) that allows boid to have different selectable settings (i.e. flock, wander, hunt, flee, etc.)
	//^can add avoidance hierarchy to this so that when a boid is going to hit an obstacle it becomes the priority of steering
};
