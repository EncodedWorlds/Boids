// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//An actor you can place in the world that spawns a set number of Boids in a Volume and headed in the direction of the flow Arrow.
//Allows the emulation of small burst of Boids exiting a finite space (i.e. nest) or a continuous flow of Boids similar to an enormous flock leaving a cave or directional migration over vast area.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumeSpawner.generated.h"

//forward declares
class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;
class ABoid;
class AFlockManager;

//spawner type enum
//TODO: move to separate file for use with other boid spawners
UENUM()
enum SpawnerType
{
	Burst     UMETA(DisplayName = "Burst"),	//spawns finite burst of boids over a small range of frames
	Flow      UMETA(DisplayName = "Flow"),	//spawns infinite amount of boids with a given rate of flow
	//TODO: could add more complex spawning for "pulsing", "finite flow", "hybrid", etc.
};

UCLASS()
class BOIDS_API AVolumeSpawner : public AActor
{
	GENERATED_BODY()

public:
	//default constructor
	AVolumeSpawner();

protected:
	//called on level start or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
protected:
	//box component used to spawn boids inside volume
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UBoxComponent* VolumeSpawnerCollision;

	//arrow component used to control the direction of flow of the spawning boids
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UArrowComponent* VolumeSpawnerFlowArrow;

	//static mesh component used for visuals of boid volume spawning actor
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UStaticMeshComponent* VolumeSpawnerMesh;

	//SPAWNING
protected:
	//type of spawning (i.e. burst, flow, etc.)
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	TEnumAsByte<SpawnerType> SpawnType;

	//number of boids to spawn initially or per interval for flow spawning
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	int32 NumBoidsToSpawn;

	//interval between flow spawns
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")	//TODO: add clamping to reasonable range
	float SpawnInterval;

	//timer handle and delegate for periodic spawning
	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnTimerDelegate;

	//type of boid to spawn
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	TSubclassOf<ABoid> BoidType;

	//spawn flock of boids at point
	UFUNCTION()
	void SpawnBoids(int32 NumBoids);

	//flock manager that has been assigned to spawned boids
	UPROPERTY(EditAnywhere, Category = "Boid|Spawn")
	AFlockManager* AssignedFlockManager;

	//TODO: add ramped spawning feature, allowing boids to be spawned over duration instead of all at once to reduce processing load on single frame to multiple frames
};
