// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//An actor that can be placed in the world to spawn and contain Boids in a designated area.
//Boids that leave the cage boundary are teleported to the other side, similar to the game Asteroids.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidCageSpawner.generated.h"

//forward declares
class UBoxComponent;
class UStaticMeshComponent;
class ABoid;
class AFlockManager;

UCLASS()
class BOIDS_API ABoidCageSpawner : public AActor
{
	GENERATED_BODY()

public:
	//default constructor used to set initial properties
	ABoidCageSpawner();

protected:
	//setup logic called when level starts or spawned
	virtual void BeginPlay() override;

	//COMPONENTS
protected:
	//collision component used to keep all boids within the cage boundaries
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boid|Components")
	UBoxComponent* CageCollision;

	//static mesh component used for visuals of cage
	//TODO: create custom SM in blender for use as base mesh and custom collision for cage wall avoidance
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UStaticMeshComponent* CageMesh;

	//CAGE OVERLAP EVENTS
protected:
	//OnEndOverlap event delegate
	//checks if a boid has escaped the cage bounds and if so places them on the opposite side within the cage (similar to the game Asteroids)
	//TODO: method assumes no rotation of the cage has been made in editor (possible to add more advance checks if more robust implementations are needed)
	UFUNCTION()
	void OnCageOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//TODO: add different functionality for handling exits such as (reflecting boid, deleting and new random spawn, etc.)

	//BOID SPAWNING
public:
	//the number of boids to spawn for the initial flock in the cage
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn", meta = (ClampMin = "0", ClampMax = "500"))
	int32 NumBoidsToSpawn;

	//type of boid to spawn
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	TSubclassOf<ABoid> BoidType;

	//spawn a swarm of boids in the cage
	void SpawnBoids(int NumBoids);

	//flock manager that has been assigned to spawned boids
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	AFlockManager* AssignedFlockManager;

	//TODO: add ramped spawning feature, allowing boids to be spawned over duration instead of all at once to reduce processing load on single frame to multiple frames
};
